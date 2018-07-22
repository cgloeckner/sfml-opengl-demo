#include <iostream>

#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>

class Camera3d {
	private:
		sf::Window& window;
		sf::Vector3f pos, dir_into, dir_right, dir_up, rot;
		
	public:
		Camera3d(sf::Window& window);
		
		void setPosition(sf::Vector3f const & vec);
		
		void moveInto(float delta);
		void moveRight(float delta);
		void moveUp(float delta);
		
		void rotateUp(float delta);
		
		void rotate(sf::Vector3f const & vec);
		
		sf::Vector3f getPosition() const;
		
		void operator()() const;
};

Camera3d::Camera3d(sf::Window& window)
	: window{window}
	, pos{}
	, dir_into{-1.f, 0.f, 0.f}
	, dir_right{0.f, 1.f, 0.f}
	, dir_up{0.f, 0.f, 1.f}
	, rot{} {
}

void Camera3d::setPosition(sf::Vector3f const & vec) {
	pos = vec;
}

void Camera3d::moveInto(float delta) {
	pos += dir_into * delta;
}

void Camera3d::moveRight(float delta) {
	pos += dir_right * delta;
}

void Camera3d::moveUp(float delta) {
	pos += dir_up * delta;
}

void Camera3d::rotateUp(float delta) {
	// rotate vectors around y-axis
	sf::Transform rotation;
	rotation.rotate(0., delta, 0.f);
	auto matrix = rotation.getMatrix();
	// todo: rotate dir_up and dir_into
}

void Camera3d::rotate(sf::Vector3f const & vec) {
	rot += vec;
}

sf::Vector3f Camera3d::getPosition() const {
	return pos;
}

void Camera3d::operator()() const {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.6, 1.6, -1.2, 1.2, 1.5, 50.5); // todo: use window
	
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);
	
	// move camera to position
	glTranslatef(-pos.x, -pos.y, -pos.z);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Vertex(sf::Vector3f const & vec, sf::Color const & col) {
	glColor3f(col.r / 255.f, col.g / 255.f, col.b / 255.f);
	glVertex3f(vec.x, vec.y, vec.z);
}

#include <imgui.h>
#include <imgui-SFML.h>

int main(int argc, char** argv) {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	
	sf::RenderWindow window{{800u, 600u}, "Vector3D Demo", sf::Style::Default, settings};
	window.setFramerateLimit(60);
	
	settings = window.getSettings();
	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
	
	ImGui::SFML::Init(window);
	
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	
	Camera3d cam{window};
	cam.setPosition({5.f, 0.f, 0.f});
	cam.rotate({-90.f, 0.f, -90.f}); // adjust cam for school-specific view

	sf::Vector3f a{2.f, 0.f, 0.f}, b{0.f, 1.f, 0.f}, c{0.f, -1.f, 0.f}, d{0.f, 0.f, 1.f};
	
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}

		//sf::Vector3f rot;
		float const speed = 0.1f;
		float const angle = 5.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { cam.moveRight(-speed); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { cam.moveRight( speed); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { cam.moveUp( speed); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { cam.moveUp(-speed); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { cam.moveInto( speed); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { cam.moveInto(-speed); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { cam.rotateUp( angle); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { cam.rotateUp(-angle); }
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  { rot.z =  1.f; }
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { rot.z = -1.f; }
		//cam.rotate(rot);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cam();
		
		glBegin(GL_LINES);
			Vertex({-1.f,  0.f,  0.f}, sf::Color::Yellow);
			Vertex({ 1.f,  0.f,  0.f}, sf::Color::Red);
			
			Vertex({ 0.f, -1.f,  0.f}, sf::Color::Cyan);
			Vertex({ 0.f,  1.f,  0.f}, sf::Color::Blue);
			
			Vertex({ 0.f,  0.f, -1.f}, sf::Color::Green);
			Vertex({ 0.f,  0.f,  1.f}, sf::Color::White);
		glEnd();

		glBegin(GL_TRIANGLES);
			Vertex(a, sf::Color::Red);
			Vertex(b, sf::Color::Red);
			Vertex(c, sf::Color::Red);
			
			Vertex(a, sf::Color::Blue);
			Vertex(b, sf::Color::Blue);
			Vertex(d, sf::Color::Blue);
			
			Vertex(a, sf::Color::Green);
			Vertex(d, sf::Color::Green);
			Vertex(c, sf::Color::Green);
			
			Vertex(b, sf::Color::Magenta);
			Vertex(c, sf::Color::Magenta);
			Vertex(d, sf::Color::Magenta);
		glEnd();

		window.pushGLStates();
		window.resetGLStates();
		
		ImGui::SFML::Update(window, clock.restart());
		
		ImGui::Begin("Camera");
		{
			auto pos = cam.getPosition();
			auto pos_str = "Position <" + std::to_string(pos.x) + "; " + std::to_string(pos.y)
				+ "; " + std::to_string(pos.z) + ">";
			ImGui::Text("%s", pos_str.c_str());
		}
		ImGui::End();
			
		ImGui::SFML::Render(window);
		window.popGLStates();
		
		window.display();
	}
	
	//ImGui::SFML::Shutdown();
}


#include <iostream>

#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

//#include <imgui.h>
//#include <imgui-SFML.h>

int main(int argc, char** argv) {
	sf::Window window{{800u, 600u}, "Vector3D Demo"};
	window.setFramerateLimit(60);
	
	sf::RenderTexture target;
	target.create(800u, 600u);
	
	glEnable(GL_TEXTURE_2D);
	
	//ImGui::SFML::Init(window, target);
	
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//ImGui::SFML::Update(window, target, clock.restart());
		
		//ImGui::SFML::Render(target);
		
		window.display();
	}
	
	//ImGui::SFML::Shutdown();
}


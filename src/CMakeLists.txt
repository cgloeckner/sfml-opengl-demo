cmake_minimum_required (VERSION 3.0)
project (vector3d)

if(NOT(DEFINED UNIT_TEST))
	# build main 
	add_executable(vector3d main.cpp)
	
	# link external libs
	target_link_libraries(vector3d ${FULL_DEPS} X11)
endif()

# macro for building a module
macro(build_module module)
	# query all *.cpp files
	file(GLOB_RECURSE src "${module}/*.cpp")
	
	# build lib
	add_library(${module} ${src})
endmacro(build_module)

# build all modules
#build_module(engine)

# ----------------------------------------------------------------------------------------
# Setup Module Dependencies to Grant Build Order -----------------------------------------

# setup module dependencies
#add_dependencies(viewer		engine)
#add_dependencies(viewer		parser)


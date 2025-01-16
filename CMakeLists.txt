cmake_minimum_required(VERSION 3.10)
project(My3DGame)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Set OpenGL preference
set(OpenGL_GL_PREFERENCE "GLVND")

# Find OpenGL and other libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED) # Add this line to find FreeGLUT

# Include directories
include_directories(include)             # Include your project headers

# Add the source files for your project
add_executable(My3DGame 
    src/main.cpp 
    src/movement.cpp 
    src/renderer.cpp 
    src/globals.cpp 
    src/cursor.cpp
    src/glad.c
    src/input.cpp
    src/godmode.cpp
)

# Link against OpenGL, GLEW, GLFW, FreeGLUT
target_link_libraries(My3DGame 
    ${OPENGL_LIBRARIES} 
    GLEW::GLEW 
    glfw 
    GLUT::GLUT  # Link to FreeGLUT
)

# Optional: Print configuration summary
message(STATUS "Using OpenGL: ${OPENGL_VERSION}")
message(STATUS "Using GLEW: ${GLEW_VERSION}")
message(STATUS "Using GLFW: ${GLFW_VERSION}")

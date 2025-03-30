cmake_minimum_required(VERSION 3.10)
project(My3DGame)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Set OpenGL preference
set(OpenGL_GL_PREFERENCE "GLVND")

# Find OpenGL and other libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/third_party
)

# Source files by category
set(CORE_SOURCES
    src/core/main.cpp
    src/core/globals.cpp
    src/core/godmode.cpp
)

set(GRAPHICS_SOURCES
    src/graphics/renderer.cpp
    src/graphics/lights.cpp
    src/graphics/models.cpp
)

set(INPUT_SOURCES
    src/input/input.cpp
    src/input/movement.cpp
)

set(UI_SOURCES
    src/ui/crosshair.cpp
    src/ui/cursor.cpp
    src/ui/fps_counter.cpp
)

set(THIRD_PARTY_SOURCES
    src/glad.c
)

# Combine all source files
set(SOURCES
    ${CORE_SOURCES}
    ${GRAPHICS_SOURCES}
    ${INPUT_SOURCES}
    ${UI_SOURCES}
    ${THIRD_PARTY_SOURCES}
)

# Add the executable
add_executable(My3DGame ${SOURCES})

# Link libraries
target_link_libraries(My3DGame 
    ${OPENGL_LIBRARIES} 
    GLEW::GLEW 
    glfw 
    GLUT::GLUT
)

# Print configuration summary
message(STATUS "Using OpenGL: ${OPENGL_VERSION}")
message(STATUS "Using GLEW: ${GLEW_VERSION}")
message(STATUS "Using GLFW: ${GLFW_VERSION}")

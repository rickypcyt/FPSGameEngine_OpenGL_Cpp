cmake_minimum_required(VERSION 3.10)
project(My3DGame)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    ${CMAKE_SOURCE_DIR}/include/third_party/imgui
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
    src/input/editor_input.cpp
)

set(EDITOR_SOURCES
    src/editor/editor.cpp
)

set(UI_SOURCES
    src/ui/crosshair.cpp
    src/ui/cursor.cpp
    src/ui/fps_counter.cpp
    src/ui/imgui_interface.cpp
)

set(THIRD_PARTY_SOURCES
    src/glad.c
    src/third_party/imgui/imgui.cpp
    src/third_party/imgui/imgui_demo.cpp
    src/third_party/imgui/imgui_draw.cpp
    src/third_party/imgui/imgui_tables.cpp
    src/third_party/imgui/imgui_widgets.cpp
    src/third_party/imgui/backends/imgui_impl_glfw.cpp
    src/third_party/imgui/backends/imgui_impl_opengl2.cpp
)

# Add ImGui sources
set(IMGUI_SOURCES
    src/third_party/imgui/imgui.cpp
    src/third_party/imgui/imgui_demo.cpp
    src/third_party/imgui/imgui_draw.cpp
    src/third_party/imgui/imgui_tables.cpp
    src/third_party/imgui/imgui_widgets.cpp
    src/third_party/imgui/backends/imgui_impl_glfw.cpp
    src/third_party/imgui/backends/imgui_impl_opengl2.cpp
)

# Add ImGui include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/third_party/imgui
)

# Combine all source files
set(SOURCES
    ${CORE_SOURCES}
    ${GRAPHICS_SOURCES}
    ${INPUT_SOURCES}
    ${EDITOR_SOURCES}
    ${UI_SOURCES}
    ${THIRD_PARTY_SOURCES}
    ${IMGUI_SOURCES}
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

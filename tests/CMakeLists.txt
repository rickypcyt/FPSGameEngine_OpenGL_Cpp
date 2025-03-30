cmake_minimum_required(VERSION 3.14)
project(GameEngineTests LANGUAGES CXX)

# Enable testing
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)

# Add test executable
add_executable(game_engine_tests
    main_test.cpp
    fps_counter_test.cpp
    movement_test.cpp
    editor_test.cpp
)

# Link against GTest and our game engine library
target_link_libraries(game_engine_tests
    PRIVATE
    GTest::GTest
    GTest::Main
    game_engine_lib
)

# Include directories
target_include_directories(game_engine_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Add tests to CTest
add_test(NAME game_engine_tests COMMAND game_engine_tests) 
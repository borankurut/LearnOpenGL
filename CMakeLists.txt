cmake_minimum_required(VERSION 3.10)

# Set the project name here
set(PROJECT_NAME "OpenGLProject")

# Create compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use the project name variable
project(${PROJECT_NAME})

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories for GLFW and GLAD
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)

# Include directories from external libraries
include_directories(extern/glfw/include)
include_directories(extern/glad/include)

# Add src directory
include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw glad)

# Platform-specific libraries
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} X11 pthread Xrandr Xi dl GL)
endif()

# On Windows, we need to link with opengl32
if (WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()


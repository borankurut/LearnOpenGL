cmake_minimum_required(VERSION 3.10)

# Set the project name here
set(PROJECT_NAME "OpenGLProject")

# Create compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Use the project name variable
project(${PROJECT_NAME})

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories for GLFW and GLAD
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/assimp-5.4.3)

# Include directories from external libraries
include_directories(include)

# Add src and lib directories
include_directories(src)
include_directories(lib)
include_directories(Objects)

# Gather all source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE LIB_FILES "lib/*.cpp")

# Copy Shaders directory to the build directory
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Shaders ${CMAKE_BINARY_DIR}/Shaders
)

# Function to create an executable for each main cpp file
function(add_application app_name)
    add_executable(${app_name} ${ARGN} ${LIB_FILES})
    target_link_libraries(${app_name} glfw glad assimp)

    # Ensure shaders are copied before building the executable
    add_dependencies(${app_name} copy_shaders)

    # Platform-specific libraries
    if (UNIX AND NOT APPLE)
        target_link_libraries(${app_name} X11 pthread Xrandr Xi dl GL)
    endif()

    # On Windows, we need to link with opengl32
    if (WIN32)
        target_link_libraries(${app_name} opengl32)
    endif()
endfunction()

# Create an executable for each source file with a main function
foreach(src_file ${SRC_FILES})
    get_filename_component(app_name ${src_file} NAME_WE)
    add_application(${app_name} ${src_file})
endforeach()

# Define DEBUG and RELEASE build settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
else()
    add_compile_definitions(RELEASE)
endif()


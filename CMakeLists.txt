cmake_minimum_required(VERSION 3.25)
project(openGL_template)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE src src/main.cpp src/*.cpp)

add_executable(${PROJECT_NAME} ${src})



if (UNIX AND NOT APPLE) 
    set(OpenGL_GL_PREFERENCE GLVND)
endif()

find_package(OpenGL REQUIRED)

set(glew-cmake_BUILD_SHARED OFF)

add_subdirectory(dependencies/glew)
add_subdirectory(dependencies/glfw)

target_include_directories(${PROJECT_NAME} PRIVATE 
    dependencies/glfw/include
    dependencies/glew/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    libglew_static
    ${OPENGL_LIBRARIES}
)
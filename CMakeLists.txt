cmake_minimum_required(VERSION 3.5)
project(openGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



file(GLOB_RECURSE APP_SRC "${CMAKE_SOURCE_DIR}/application/src/*.cpp")
set(APP_INCLUDE "${CMAKE_SOURCE_DIR}/application/include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} ${APP_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${APP_INCLUDE})



find_package(OpenGL REQUIRED)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glfw
)
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glew
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_STATIC ON  CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glew)

target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenGL::GL
    glfw
    libglew_static
)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)
cmake_minimum_required(VERSION 3.28)

project(OpenGL_Learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(glfw)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC glfw "${GLFW_SOURCE_DIR}/deps")
target_link_libraries(${PROJECT_NAME} PUBLIC glfw ${OPENGL_LIBRARIES})
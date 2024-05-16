cmake_minimum_required(VERSION 3.28)

project(OpenGL_Learn LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")

add_executable(${PROJECT_NAME} ${ENGINE_SOURCES})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glew/build/cmake")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glm")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/glm")

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glew)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
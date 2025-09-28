cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER clang++)
project(OpenGL)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        main.cpp
        glad.c
)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)

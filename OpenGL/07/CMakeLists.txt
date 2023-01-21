cmake_minimum_required(VERSION 3.18)

project(07)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


add_subdirectory(libs/SDL-release-2.26.1)
add_subdirectory(libs/geGL)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE libs/glm-0.9.9.8)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} SDL2-static SDL2::SDL2main geGL::geGL glm)

add_custom_target(run ./${PROJECT_NAME})
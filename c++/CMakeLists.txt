cmake_minimum_required(VERSION 3.15)

project(exercises_C++)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

#add_subdirectory(libs/glfw-3.3.8)
#add_subdirectory(libs/SDL-release-2.26.1)
#add_library(glm INTERFACE)
#target_include_directories(glm INTERFACE libs/glm)


set(LIBS_SOURCES
 # libs/stb/stb_image.h
 # libs/json/json.h
  )

add_executable(${PROJECT_NAME} 
  src/main000.cpp
  src/main001.cpp
  )

#target_link_libraries(${PROJECT_NAME} glm glfw SDL2-static SDL2::SDL2main)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_target(run ./${PROJECT_NAME})



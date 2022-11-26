cmake_minimum_required(VERSION 3.20.0)
project(demo VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories(./include/)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(glm2)

set(SRC_LIST glad.c camera.h stb_image.h)

foreach(names ${SRC_LIST})
    list(APPEND filelist ${CMAKE_CURRENT_SOURCE_DIR}/src/${names})
endforeach(names)

find_library(GLFW3 names glfw3.dll HINTS ${PROJECT_SOURCE_DIR}/lib/)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
#SET(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(execNames 
main
stencitest
depthtest
fpstest
)
foreach(execName ${execNames})
    file(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/${execName}.cpp")
    add_executable(${execName} ${SOURCE} ${filelist})
    target_link_libraries(${execName} ${GLFW3})
endforeach(execName)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/game2d/ game2dSource)
add_executable(game2d ${game2dSource} ${filelist} ${resource} "./src/shaderManager.cpp")
target_link_libraries(game2d ${GLFW3})
target_link_libraries(game2d glm::glm)

add_custom_command(TARGET game2d
#COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:game2d>/shaders
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:game2d>/textures
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/levels $<TARGET_FILE_DIR:game2d>/levels)

message("chenming log ${GLFW3} in ${PROJECT_SOURCE_DIR}/lib/")
add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)




cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(exp)
set(SRC main.cpp)
set(EXN exp)
add_executable(${EXN} ${SRC})

set(GLFW_BUILD_DOCS 	OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS 	OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL 	OFF CACHE BOOL "" FORCE)

include_directories(ext/glm)

Add_subdirectory(ext/glfw)
target_link_libraries(${EXN} glfw)
add_subdirectory(ext/glad)
target_link_libraries(${EXN} glad)

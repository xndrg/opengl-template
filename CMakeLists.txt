cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(PROJECT_NAME math)

project(${PROJECT_NAME})
include_directories(include/)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/shader.cpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(extern/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(extern/glm)
target_link_libraries(${PROJECT_NAME} glm)

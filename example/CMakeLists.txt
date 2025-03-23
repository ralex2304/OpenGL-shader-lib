cmake_minimum_required(VERSION 3.16)

project(OpenGL-shader-lib-example LANGUAGES CXX C)

set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets)
set(ASSETS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/assets)

file(RELATIVE_PATH
  ASSETS_LOCATION
  ${CMAKE_INSTALL_PREFIX}/bin
  ${ASSETS_INSTALL_DIR}
)

include(../cmake/tools.cmake)

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME glfw
  VERSION 3.3.10
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.3.10
  OPTIONS "BUILD_SHARED_LIBS ON"
          "GLFW_BUILD_EXAMPLES OFF"
          "GLFW_BUILD_TESTS OFF"
          "GLFW_BUILD_DOCS OFF"
)

CPMAddPackage(
    NAME glad
    GITHUB_REPOSITORY Dav1dde/glad
    GIT_TAG v2.0.8
)

set(GLAD_SOURCES_DIR "${glad_SOURCE_DIR}")
set(GLAD_GEN_DIR "${CMAKE_BINARY_DIR}/gladgen")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad STATIC REPRODUCIBLE QUIET LOCATION ${GLAD_GEN_DIR} LOADER API gl:core=3.3)
include_directories("${GLAD_GEN_DIR}/include")

CPMAddPackage(NAME OpenGL-shader-lib SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

set(sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_PATH="${ASSETS_LOCATION}")

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 OUTPUT_NAME "OpenGL-shader-lib-example")

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw OpenGL-shader-lib::OpenGL-shader-lib)


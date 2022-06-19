cmake_minimum_required(VERSION 3.0.0)
project(MazeSim VERSION 0.1.0)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)

include_directories(lib/glm/glm/)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.*)

add_executable(MazeSim ${SRCS})

target_link_libraries(MazeSim glad)
target_link_libraries(MazeSim glfw)

# target_link_libraries(MazeSim -mwindows)

target_compile_features(MazeSim PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
cmake_minimum_required(VERSION 3.0.0)
project(MazeSim VERSION 0.1.0)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)
# find_package(imgui REQUIRED)

add_subdirectory(lib/glfw)


# Compile ImGui as static library 
set(IMGUI_PATH "lib/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES} ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH}/backends)
# target_compile_definitions("ImGui" PUBLIC IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_sources("ImGui" PRIVATE ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp)
target_sources("ImGui" PRIVATE ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)
target_link_libraries("ImGui" PUBLIC glfw)


file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.*)

add_executable(MazeSim ${SRCS})

target_link_libraries(MazeSim PUBLIC glfw)
target_link_libraries(MazeSim PUBLIC "ImGui")

# target_link_libraries(MazeSim PUBLIC -mwindows)

target_compile_features(MazeSim PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
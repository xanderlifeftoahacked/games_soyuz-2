cmake_minimum_required(VERSION 3.12)

if(UNIX)
set(CMAKE_TOOLCHAIN_FILE /home/umarnurmatov/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
endif(UNIX)

if(WIN32)
set(CMAKE_TOOLCHAIN_FILE C:/vcp/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
endif(WIN32)

project(cppgame VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/resources/")

# Find SFML shared libraries
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(cppgame
    src/cppgame.cpp)

# Set include directory search paths
target_include_directories(cppgame
  PRIVATE
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )


target_compile_definitions(cppgame PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
target_compile_definitions(cppgame PRIVATE RESOURSE_PATH="${PROJECT_SOURCE_DIR}/resourses/}")
  

target_link_libraries(cppgame
    PRIVATE ImGui-SFML::ImGui-SFML
    PRIVATE imgui::imgui
    PRIVATE sfml-system sfml-graphics sfml-window)
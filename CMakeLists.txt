cmake_minimum_required(VERSION 3.21)
project(katengine_realisms)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

message("$ENV{VULKAN_SDK}")

add_library(katengine src/kat/Engine.cpp src/kat/Renderer.cpp src/kat/Renderer.h)
target_include_directories(katengine PUBLIC src/ $ENV{VULKAN_SDK}/Include)
target_include_directories(katengine PRIVATE ${STB_INCLUDE_DIRS})
target_link_directories(katengine PUBLIC $ENV{VULKAN_SDK}/Lib)
target_link_libraries(katengine PUBLIC glfw glad::glad glm::glm spdlog::spdlog vulkan-1.lib)

add_library(kat::engine ALIAS katengine)


add_subdirectory(app)
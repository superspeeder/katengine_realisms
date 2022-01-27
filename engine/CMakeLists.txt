add_library(katengine src/kat/Engine.cpp include/kat/Engine.h src/kat/Renderer.cpp include/kat/Renderer.h)
target_include_directories(katengine PUBLIC include/ $ENV{VULKAN_SDK}/Include)
target_include_directories(katengine PRIVATE ${STB_INCLUDE_DIRS})
target_link_directories(katengine PUBLIC $ENV{VULKAN_SDK}/Lib)
target_link_libraries(katengine PUBLIC glfw glad::glad glm::glm spdlog::spdlog vulkan-1.lib)

add_library(kat::engine ALIAS katengine)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX engine/src)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX engine/include)

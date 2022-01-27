
add_executable(app src/main.cpp src/TestApp.cpp include/TestApp.h)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(app PRIVATE kat::engine)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX app/src)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX app/include)
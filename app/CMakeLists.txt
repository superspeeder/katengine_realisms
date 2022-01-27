
add_executable(app main.cpp TestApp.cpp TestApp.h)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(app PRIVATE kat::engine)
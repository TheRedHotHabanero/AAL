add_library(window window.cc)

include_directories(window PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(window PUBLIC ${SDL2_LIBRARIES})

add_library(menu menu.cc)

target_link_libraries(menu PUBLIC ${GTKMM_LIBRARIES})
target_include_directories(menu PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_directories(menu PUBLIC ${GTKMM_LIBRARY_DIRS})

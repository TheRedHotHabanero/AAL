add_library(lyapunov lyapunov.cc)

target_link_libraries(lyapunov PUBLIC ${GTKMM_LIBRARIES})
target_include_directories(lyapunov PUBLIC ${GTKMM_INCLUDE_DIRS})
target_link_directories(lyapunov PUBLIC ${GTKMM_LIBRARY_DIRS})

include_directories(lyapunov PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(lyapunov PRIVATE ${SDL2_LIBRARIES})

target_link_libraries(lyapunov PRIVATE window)
target_link_libraries(lyapunov PRIVATE menu)

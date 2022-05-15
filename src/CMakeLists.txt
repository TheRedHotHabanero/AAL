set(LIBLIST lyapunov menu region time window)

foreach(LIB ${LIBLIST})
  add_subdirectory(${LIB})

  target_include_directories(${LIB} PUBLIC ${CMAKE_SOURCE_DIR}/include)
  target_include_directories(${LIB} PUBLIC ${CMAKE_SOURCE_DIR}/include/${LIB})

  target_link_libraries(${LIB} PRIVATE Threads::Threads)

  target_sources(${LIB} PUBLIC ${CMAKE_SOURCE_DIR}/include/${LIB}/${LIB}.hh)
endforeach()

set(LIBLIST "${LIBLIST}" PARENT_SCOPE)

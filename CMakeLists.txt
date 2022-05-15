cmake_minimum_required(VERSION 3.16)
project(AALProject)

find_package(SDL2 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)

# Include compiler flags
include(cmake/comp_flags.cmake)

# Prepare project libraries
add_subdirectory(src)
message(STATUS "Collected libs: ${LIBLIST}")

foreach(LIB ${LIBLIST})
  # Include paths
  target_include_directories(${LIB} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

  target_compile_features(${LIB} PUBLIC cxx_std_17)
  apply_compiler_flags(${LIB} PUBLIC)
endforeach()

# Prepare project tools
add_subdirectory(tools)
message(STATUS "Collected tools: ${TOOLLIST}")

foreach(TOOL ${TOOLLIST})
  target_include_directories(${TOOL} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_compile_features(${TOOL} PRIVATE cxx_std_17)
  set_target_properties(${TOOL}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  )

  apply_compiler_flags(${TOOL} PRIVATE)
endforeach()

cmake_minimum_required(VERSION 3.16)
project(sim2022)

include(cmake/subdirs.cmake)

set(DIRS 3rd_party src sim)

foreach(DIR ${DIRS})
  add_subdirectory(${DIR})
endforeach()

set(TARGETS)
list(APPEND TARGETS ${SIMLIB})

foreach(TARGET IN LISTS TARGETS)
  target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/ELFIO)

  target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_features(${TARGET} PRIVATE cxx_std_17)
endforeach()

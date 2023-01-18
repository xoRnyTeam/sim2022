cmake_minimum_required(VERSION 3.16)
project(sim2022)

set(CMAKE_INCL subdirs flags upd_list)
#
foreach(INCL ${CMAKE_INCL})
  include(cmake/${INCL}.cmake)
endforeach()

enable_testing()

set(DIRS 3rd_party src sim test)
foreach(DIR ${DIRS})
  add_subdirectory(${DIR})
endforeach()

set(TARGETS)
list(APPEND TARGETS ${SIM} ${SIMLIB})

foreach(TARGET IN LISTS TARGETS)
  target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/ELFIO)

  # target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
  apply_compiler_flags(${TARGET} PRIVATE)
  target_compile_features(${TARGET} PRIVATE cxx_std_20)
endforeach()

foreach(TARGET ${SIM})
  target_link_libraries(${TARGET} PUBLIC ${SIMLIB} decoder)
endforeach()

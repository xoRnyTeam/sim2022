SUBDIRS(DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(SIMLIB ${DIRS})

foreach(TARGET ${DIRS})
    add_subdirectory(${TARGET})
endforeach() 

set(SIMLIB ${SIMLIB} PARENT_SCOPE)

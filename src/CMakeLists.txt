SUBDIRS(DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(SIMLIB ${DIRS})

foreach(TARGET ${DIRS})
    add_library(${TARGET} ${TARGET}/${TARGET}.cpp)

    # NOTE: maybe add it single cmake for every .cpp if it will be usefull
    # add_subdirectory(${DIR})
endforeach() 

set(SIMLIB ${SIMLIB} PARENT_SCOPE)

set(TARGETS memory)

foreach(TARGET ${TARGETS})
    add_library(${TARGET} ${TARGET}.cpp)
endforeach()

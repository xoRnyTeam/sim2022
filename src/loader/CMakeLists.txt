set(TARGETS loader)

foreach(TARGET ${TARGETS})
    add_library(${TARGET} ${TARGET}.cpp)
endforeach()

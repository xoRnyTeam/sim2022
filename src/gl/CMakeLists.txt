set(TARGET gl)
add_library(${TARGET} gl.cpp)
target_link_libraries(${TARGET} sfml-system sfml-graphics sfml-window)
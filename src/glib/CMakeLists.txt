set(TARGET glib)
add_library(${TARGET} SEngine.cpp)

target_link_libraries(${TARGET} sfml-system sfml-graphics sfml-window)

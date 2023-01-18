set(TARGET driver)
add_library(${TARGET} ${TARGET}.cpp)

target_link_libraries(${TARGET} PRIVATE asmjit)
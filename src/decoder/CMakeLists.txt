set(TARGET decoder)
set(RISC_SCR ${CMAKE_SOURCE_DIR}/3rd_party/riscv-opcodes)
set(CODEGEN_LOG ${CMAKE_CURRENT_SOURCE_DIR}/log.out)
#
find_package(Python3 COMPONENTS Interpreter REQUIRED)
#
add_custom_target(codegen ALL)

add_custom_command(
    TARGET codegen PRE_BUILD
    COMMAND make all
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/codegen
    COMMENT "Codegen of decoder & other stuff from RISC-V sources"
)
#
add_library(${TARGET} ${TARGET}.cpp)
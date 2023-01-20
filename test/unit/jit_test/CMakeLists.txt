set(TARGET jit_test)
add_executable(${TARGET} ${TARGET}.cc jit_test_2.cpp)
#
UPD_LIST(${TARGET} TESTLIST)
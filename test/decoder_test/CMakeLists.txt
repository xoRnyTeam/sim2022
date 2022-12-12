set(TARGET decoder_test)
set(TESTLIST ${TARGET})

add_executable(${TARGET} ${TARGET}.cc)
set(TESTLIST ${TESTLIST} PARENT_SCOPE)
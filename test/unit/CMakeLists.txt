if(BUILD_TESTS)
    #
    find_package(GTest REQUIRED)

    SUBDIRS(DIRS ${CMAKE_CURRENT_SOURCE_DIR})
    # set(DIRS decoder_test)
    set(TESTLIST)

    foreach(DIR ${DIRS})
        add_subdirectory(${DIR})
    endforeach()
    #
    foreach(TEST ${TESTLIST})
        target_include_directories(${TEST} PRIVATE ${CMAKE_SOURCE_DIR}/include)
        target_include_directories(${TEST} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party)
        target_include_directories(${TEST} PRIVATE ${GTEST_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
        target_include_directories(${TEST} PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/asmjit/src)

        #
        target_link_libraries(${TEST} PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)
        target_link_libraries(${TEST} PRIVATE asmjit)
        target_link_libraries(${TEST} PRIVATE ${SIMLIB})

        #
        target_compile_features(${TEST} PRIVATE cxx_std_20)
        set_target_properties(${TEST}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test"
        )
    endforeach()

    foreach(TEST ${TESTLIST})
        message("current test = ${TEST}")
        add_test(NAME ${TEST} COMMAND ${TEST} --gtest_color=yes)
        set_property(TEST ${TEST} PROPERTY LABELS unit)
    endforeach()

#
endif()

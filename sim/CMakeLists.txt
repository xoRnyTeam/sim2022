set(SIM simulator)

add_executable(${SIM} main.cpp)
target_link_libraries(${SIM} PRIVATE CLI11::CLI11)
target_include_directories(${SIM} SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/CLI11/include)

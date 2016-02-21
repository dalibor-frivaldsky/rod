cmake_minimum_required(VERSION 3.4)

include( CTest )

include_directories(
    ../../hana/include
)

# Tests
add_executable( test1 test/unit/test.cpp )
add_test( test1 test1 )
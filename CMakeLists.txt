cmake_minimum_required(VERSION 3.11)

project(metafjson)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -fcxx-exceptions")

set(SOURCE src/main.cpp)

include_directories(
    include
    metaf/include
    cxxopts/include
    json/include
    magic_enum/include
    date/include
)

# Build program

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/commandlineargs.cpp 
    src/datetimeformat.cpp 
    src/outputformat.cpp 
    src/outputformatbasic.cpp 
    src/settings.cpp 
    src/utility.cpp 
    src/valueformat.cpp 
    )

# Tests

add_executable(test 
    src/commandlineargs.cpp 
    src/datetimeformat.cpp 
    src/outputformat.cpp 
    src/outputformatbasic.cpp 
    src/settings.cpp 
    src/utility.cpp 
    src/valueformat.cpp 
    googletest/googletest/src/gtest-all.cc
    test/main.cpp
    test/test_commandlineargs.cpp
    test/test_datetimeformat.cpp
    test/test_valueformat.cpp
)

target_include_directories(test PRIVATE 
    googletest/googletest
    googletest/googletest/include)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(TEST_LINK_FLAGS "-lpthread -lm")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(TEST_LINK_FLAGS "-pthread")
endif()	

set_target_properties(test PROPERTIES 
    LINK_FLAGS ${TEST_LINK_FLAGS})
cmake_minimum_required(VERSION 3.26)
project(longArithmetic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LongArithmetic SHARED
        lib/longArithmetic.h
        lib/longArithmetic.cpp
)

add_executable(pi calculationofpi/piCalculation.cpp)
target_link_libraries(pi LongArithmetic)


add_executable(test tests/testOfArithmetics.cpp)
target_link_libraries(test LongArithmetic)


add_custom_target(runpi COMMAND pi)
add_custom_target(runtest COMMAND test)



cmake_minimum_required(VERSION 3.20)
project(SDM-Exercise2 CXX)

add_executable(exercise2 main.cpp company_name.cpp utils.cpp)
set_target_properties(exercise2 PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_compile_options(exercise2 PRIVATE
    $<$<CONFIG:DEBUG>:-Og -g3 -Wall -Wextra -Wpedantic -Werror>
    $<$<CONFIG:RELEASE>:-O3 -Wall -Wextra -Wpedantic -Werror>
)
set_property(TARGET exercise2 PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
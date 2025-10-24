cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(common STATIC)

target_sources(common 
    PRIVATE 
        src/vector.cpp
        src/scene_parser.cpp
        src/config_parser.cpp
        src/parser_utilities.cpp
        src/hittable.cpp
        src/geometry_logic.cpp
        src/material_logic.cpp
        src/math_utilities.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(common PUBLIC Microsoft.GSL::GSL) 

#Ejecutable para probar el parser de archivos
add_executable(test_parser src/test_parser.cpp)
target_link_libraries(test_parser common)
target_include_directories(test_parser PRIVATE include)
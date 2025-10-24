# Versión mínima de CMake
cmake_minimum_required(VERSION 3.22)

# Nombre del proyecto
project(aos_renderer CXX)

# Usamos C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Librería AOS
add_library(aos_lib STATIC
    src/main.cpp
)

target_include_directories(aos_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(aos_lib PUBLIC common)

# Ejecutable render-aos (solo si ya tienes main.cpp)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(render-aos src/main.cpp)
    target_link_libraries(render-aos PRIVATE aos_lib)
endif()


#add_library(aos_lib STATIC)

#target_sources(aos_lib
#    PRIVATE
#        src/main.cpp
#)

#target_include_directories(aos_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(aos_lib PUBLIC common Microsoft.GSL::GSL)

# Ejecutable render-aos (programa principal)
#add_executable(render-aos src/main.cpp)
#target_link_libraries(render-aos PRIVATE aos_lib)


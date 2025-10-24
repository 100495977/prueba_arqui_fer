cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Librería SOA
add_library(soa_lib STATIC
    src/soa_image.cpp
    src/soa_color.cpp
    src/soa_camera.cpp
    src/soa_ray.cpp
    src/render_soa.cpp
)

target_include_directories(soa_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(soa_lib PUBLIC common)

# Ejecutable render-soa (solo si ya tienes main.cpp)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(render-soa src/main.cpp)
    target_link_libraries(render-soa PRIVATE soa_lib)
endif()

#add_library(soa_lib STATIC)


#target_sources(render-soa
#    PRIVATE
#      src/main.cpp
#      soa_image.cpp
#)
#target_include_directories(render-soa PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(render-soa PRIVATE Microsoft.GSL::GSL common)

#add_executable(render-soa src/main.cpp)
#target_link_libraries(render-soa PRIVATE soa_lib)
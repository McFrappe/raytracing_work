 cmake_minimum_required(VERSION 3.22)
project (Raycast)

find_package(JPEG REQUIRED)
find_package(OpenMP)

add_executable(Raycast main.cpp)


if(OpenMP_CXX_FOUND)
    message("OpenMP found")
    add_compile_definitions(OMP)
    target_link_libraries(Raycast PUBLIC OpenMP::OpenMP_CXX
                                                            ${JPEG_LIBRARIES})
else()
    message(WARNING "OpenMP not found")
    target_link_libraries(Raycast ${JPEG_LIBRARIES})
endif()
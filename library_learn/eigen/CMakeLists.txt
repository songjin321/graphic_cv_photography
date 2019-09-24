cmake_minimum_required( VERSION 2.8 )
project( eigen_geometry )
add_compile_options(-std=c++11)
find_package(Eigen3 REQUIRED)

include_directories(
    ${EIGEN3_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
    geometry.cpp
)
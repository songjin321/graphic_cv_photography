cmake_minimum_required( VERSION 2.8 )
project( ceres_view )
add_compile_options(-std=c++11)
find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)

include_directories(
    ${EIGEN3_INCLUDE_DIRS}
    ${CERES_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
    ceres_view.cpp
)

target_link_libraries(${PROJECT_NAME} ${CERES_LIBRARIES})
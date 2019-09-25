cmake_minimum_required( VERSION 2.8 )
project( opencv_test )
add_compile_options(-std=c++11)
find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
    assignment.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
)
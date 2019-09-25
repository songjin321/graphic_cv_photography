cmake_minimum_required(VERSION 2.8)

project(cudaDetector)

find_package(OpenCV REQUIRED)
find_package(CUDA)

if (NOT CUDA_FOUND)
  message(STATUS "CUDA not found. Project will not be built.")
endif(NOT CUDA_FOUND)


include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

message(${OpenCV_INCLUDE_DIRS})

cuda_add_executable(cudafast 
    fast.cu
    mainFast.cpp
    OPTIONS -arch=sm_35
)
target_link_libraries(cudafast 
    ${CUDA_cudadevrt_LIBRARY} 
    ${OpenCV_LIBS}
)
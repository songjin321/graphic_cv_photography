project(sift_test)
cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(catkin_simple REQUIRED)
catkin_simple(ALL_DEPS_REQUIRED)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    sensor_msgs
    cv_bridge
)
find_package(OpenCV REQUIRED)

find_package(CUDA)
if (NOT CUDA_FOUND)
  message(STATUS "CUDA not found. Project will not be built.")
endif(NOT CUDA_FOUND)

if (WIN32)
  set(EXTRA_CXX_FLAGS "/DVERBOSE /D_CRT_SECURE_NO_WARNINGS ")
  list(APPEND CUDA_NVCC_FLAGS "-arch=sm_35;--compiler-options;-O2;-DVERBOSE") 
endif()
if (UNIX)
  if (APPLE)
    set(EXTRA_CXX_FLAGS "-DVERBOSE -msse2")
    list(APPEND CUDA_NVCC_FLAGS "-arch=sm_35;--compiler-options;-O2;-DVERBOSE") 
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -msse2 ")
    list(APPEND CUDA_NVCC_FLAGS "-lineinfo;-ccbin;/usr/bin/gcc-5;--compiler-options;-O2;-D_FORCE_INLINES;-DVERBOSE_NOT") 
  endif()
endif()

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}
    CudaSift
    )

set(cuda_sources
  CudaSift/cudaImage.cu  
  CudaSift/cudaImage.h  
  CudaSift/cudaSiftH.cu 
  CudaSift/cudaSiftH.h  
  CudaSift/matching.cu  
  CudaSift/cudaSiftD.h  
  CudaSift/cudaSift.h  
  CudaSift/cudautils.h
)

cuda_add_executable(sift_test_node 
src/sift_test_node.cpp 
${cuda_sources}
)

target_link_libraries(sift_test_node
 ${CUDA_cudadevrt_LIBRARY} 
 ${OpenCV_LIBS}
 ${catkin_LIBRARIES}
)

cs_install()
cs_export()
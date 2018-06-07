CMAKE_MINIMUM_REQUIRED (VERSION 2.8.3)
SET(PROJECT_NAME CoordinateConvert)
PROJECT(${PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-g -std=c++11")
#-DEIGEN_USE_MKL_ALL")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

find_package(PROJ4 REQUIRED) #配置proj.4库
find_package(Eigen3 REQUIRED)

# Include dirs
INCLUDE_DIRECTORIES(
   ${PROJ4_INCLUDE_DIRS}
   ${EIGEN3_INCLUDE_DIRS}
)

add_executable(CoordinateConvert  
			coordinate_convert_tool.cpp
			MainTest.cpp	
)
target_link_libraries(CoordinateConvert ${PROJ4_LIBRARIES})





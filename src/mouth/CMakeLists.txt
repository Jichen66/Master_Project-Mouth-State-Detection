cmake_minimum_required(VERSION 2.8.3)
project(mouth)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)



add_subdirectory(/home/jichen/dlib-19.17/dlib dlib_build)

find_package(OpenCV REQUIRED)


file(GLOB_RECURSE HEADERS include/mouth/*.h)


## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  #cv_bridge
  image_transport
  message_generation
  message_runtime
  roscpp
  sensor_msgs
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rail_track
 #CATKIN_DEPENDS image_transport message_generation message_runtime roscpp sensor_msgs std_msgs
#  DEPENDS system_lib
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}
  ~/DLIB/dlib-19.13/
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
 #  src/FaceDetector.cpp
#   src/MouthStateClassifier.cpp
 #  src/MouthStateDetector.cpp
 #  src/MouthTracker.cpp
 #)



#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(mouthstate src/main.cpp
  src/FaceDetector.cpp
   src/MouthStateClassifier.cpp
  src/MouthStateDetector.cpp
  src/MouthTracker.cpp
  ${HEADERS}
)

add_dependencies(mouthstate ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})

target_link_libraries(mouthstate
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
    dlib::dlib
  #  ${PROJECT_NAME}

)



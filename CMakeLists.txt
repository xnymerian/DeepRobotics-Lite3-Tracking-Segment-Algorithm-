cmake_minimum_required(VERSION 3.10)
project(lite3_tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/network.cpp
    src/model_loader.cpp
    src/detector.cpp
    src/visualizer.cpp
    src/robot_controller.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    rknnrt
    dl
    pthread
)

# Copy model and library files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/yolo11n-seg-320.rknn DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/librga.so DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/librknnrt.so DESTINATION ${CMAKE_BINARY_DIR})


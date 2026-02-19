cmake_minimum_required(VERSION 3.10)
project(ImageRetrieval CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
    message(STATUS "OpenCV found: ${OpenCV_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "OpenCV not found!")
endif()

# Source files
set(SOURCES
    Driver.cpp
    Filter.cpp
    baseline.cpp
    histogram.cpp
    law.cpp
    multihistogram.cpp
    textureColor.cpp
)

# Create executable
add_executable(image_retrieval ${SOURCES})

# Link OpenCV
target_link_libraries(image_retrieval ${OpenCV_LIBS})

# Include directories
target_include_directories(image_retrieval PRIVATE ${OpenCV_INCLUDE_DIRS})

# Print usage info
message(STATUS "")
message(STATUS "Image Retrieval Project - Build Complete!")
message(STATUS "Usage: ./image_retrieval <Target_File> <Dir_name> <Type> <Number_of_results>")
message(STATUS "")

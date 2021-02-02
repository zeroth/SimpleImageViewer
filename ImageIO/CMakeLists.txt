cmake_minimum_required(VERSION 3.10)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_subdirectory(Tiff)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

# find_package(Qt5Charts)
find_package(OpenMP)
if (OPENMP_FOUND)
    message(STATUS "found OpenMP")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

SET(LOCAL_TIFF_IO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${LOCAL_TIFF_IO_DIR})
message("Local include ")
message(${LOCAL_TIFF_IO_DIR})
# all source files
# file(GLOB SRC "*.h" "*.cpp" )


add_library(ImageIO 
BioImage.h
BioImage.cpp
#Histogram.h
#Histogram.cpp
)
target_include_directories(ImageIO PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR} ${LOCAL_TIFF_IO_DIR})
# target_link_libraries(SimpleImageViewer PUBLIC ImageIO)

target_link_libraries(ImageIO 
    PRIVATE  LocalTiffIO Qt5::Core Qt5::Gui)


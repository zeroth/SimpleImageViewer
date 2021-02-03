cmake_minimum_required(VERSION 3.10)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# find_package(Qt5Charts)

# Lib TIFF
# Tiff lib
IF (WIN32)

    FIND_LIBRARY(TIFF_LIBRARIES tiff
        HINTS
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tifflib/lib
        REQUIRED)

    SET(TIFF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tifflib/include )
    SET(TIFF_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tifflib/lib)
    INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIR})
    LINK_DIRECTORIES(${TIFF_LIBRARY_DIR})
    message("Tiff include dir")
    message(${TIFF_INCLUDE_DIR})
ELSE()
    # linux make sure you install
    # sudo apt-get install -y libtiff-dev
    FIND_LIBRARY(TIFF_LIBRARIES tiff)
ENDIF()

# all source files
file(GLOB SRC "*.h" "*.cpp")


add_library(LocalTiffIO ${SRC})
target_include_directories(LocalTiffIO PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR} ${TIFF_INCLUDE_DIR})
# target_link_libraries(SimpleImageViewer PUBLIC ImageIO)

target_link_libraries(LocalTiffIO 
    PRIVATE  ${TIFF_LIBRARIES})

IF (WIN32)
add_custom_command(TARGET LocalTiffIO
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy  ${TIFF_LIBRARY_DIR}/tiff.dll "${CMAKE_CURRENT_BINARY_DIR}/../../Viewer2D"
)
#add_custom_command(TARGET LocalTiffIO
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy  ${TIFF_LIBRARY_DIR}/tiff.dll "${CMAKE_CURRENT_BINARY_DIR}/../../ImageIO"
#)
ENDIF()

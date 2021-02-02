cmake_minimum_required(VERSION 3.10)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)
# find_package(Qt5Charts)

# all source files
file(GLOB SRC "*.h" "*.cpp" "*.qrc")


add_executable(ImageIOTest ${SRC})
# target_link_libraries(SimpleImageViewer PUBLIC ImageIO)
target_compile_definitions(ImageIOTest
    PRIVATE QT_QML_DEBUG)
target_link_libraries(ImageIOTest
    PRIVATE ImageIO Qt5::Core Qt5::Gui )


IF (WIN32)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        SET(WINDEPLOY_TYPE "debug")
    else()
        SET(WINDEPLOY_TYPE "release")
    endif()
    message(${CMAKE_CURRENT_BINARY_DIR})

    message(STATUS ${WINDEPLOY_TYPE})
    
    get_filename_component(QT_BASE_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)

    add_custom_command(TARGET ImageIOTest
        POST_BUILD
        COMMAND ${QT_BASE_DIR}/windeployqt.exe --${WINDEPLOY_TYPE} --compiler-runtime  --json --pdb --qmldir ${QT_BASE_DIR}/../qml "${CMAKE_CURRENT_BINARY_DIR}/ImageIOTest.exe"
    )

    
ENDIF()

cmake_minimum_required(VERSION 3.4)
project(spectacular)

set(QTDIR "C:\\Qt\\5.5\\mingw492_32")
set(QT_INCLUDE "${QTDIR}\\include")
set(QT_BIN "${QTDIR}\\bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(3rdparty/yaml-cpp)

include_directories(${QT_INCLUDE}
                    include/core)
link_directories(${QT_BIN}
                3rdparty/yaml-cpp)

set(SOURCE_FILES
        src/core/APIDataReceiver.cpp
        src/core/CommonStructures.cpp
        src/core/SessionStrParsingUtils.cpp
        src/irsdk_utils.cpp
        src/main.cpp)

add_executable(spectacular ${SOURCE_FILES})
target_link_libraries(spectacular Winmm.lib yaml-cpp Qt5Core.dll)

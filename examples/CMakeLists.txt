cmake_minimum_required (VERSION 3.0)
project (example)

ADD_DEFINITIONS(
	-fpermissive
    -std=c++11
    -D EXECU
)

file(GLOB SOURCES
    ../microsoft/cognitive/cv/*.h
    ../microsoft/cognitive/cv/*.cpp
	../microsoft/utils/*.h
    ../microsoft/utils/*.cpp
)

add_executable(example example01.cpp ${SOURCES})

include_directories(".")
include_directories("../")

find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(example ${CURL_LIBRARIES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
target_link_libraries(example ${JSONCPP_LIBRARIES})

message(STATUS "${JPEG_LIBRARIES}")
message(STATUS "${JPEG_LIBRARY}")
message(STATUS "${JPEG_FOUND}")
message(STATUS "${JPEG_INCLUDE_DIR}")
message(STATUS "${CURL_INCLUDE_DIR}")
message(STATUS "${CURL_LIBRARIES}")
message(STATUS "${JSONCPP_INCLUDE_DIRS}")
message(STATUS "${JSONCPP_LIBRARIES}")
message(STATUS "${JSONCPP_CFLAGS}")
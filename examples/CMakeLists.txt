cmake_minimum_required (VERSION 3.0)
project (cognitive_services)

ADD_DEFINITIONS(
	-Wformat-security
	-fpermissive
    -std=c++11
    -D EXECU
)

file(GLOB SOURCES
    ../microsoft/cognitive/cv/*.h
    ../microsoft/cognitive/cv/*.cpp
	../microsoft/utils/*.h
    ../microsoft/utils/*.cpp	
	../gphoto2-cpp/gphoto2.h
)

add_executable(example example01.cpp ${SOURCES})
add_executable(camera-ai camera-ai.cpp ${SOURCES} ../gphoto2-cpp/gphoto2.cpp)

include_directories(".")
include_directories("../")

find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(example ${CURL_LIBRARIES})
target_link_libraries(camera-ai ${CURL_LIBRARIES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
target_link_libraries(example ${JSONCPP_LIBRARIES})
target_link_libraries(camera-ai ${JSONCPP_LIBRARIES})

include(FindGphoto2.cmake)

IF(GPHOTO2_FOUND)
include_directories(${GPHOTO2_INCLUDE_DIR})
target_link_libraries(camera-ai ${GPHOTO2_LIBRARY} ${GPHOTO2_PORT_LIBRARY})
ENDIF(GPHOTO2_FOUND)

message(STATUS "${JPEG_LIBRARIES}")
message(STATUS "${JPEG_LIBRARY}")
message(STATUS "${JPEG_FOUND}")
message(STATUS "${JPEG_INCLUDE_DIR}")
message(STATUS "${CURL_INCLUDE_DIR}")
message(STATUS "${CURL_LIBRARIES}")
message(STATUS "${JSONCPP_INCLUDE_DIRS}")
message(STATUS "${JSONCPP_LIBRARIES}")
message(STATUS "${JSONCPP_CFLAGS}")
message(STATUS "${GPHOTO2_INCLUDE_DIR}")
message(STATUS "${GPHOTO2_LIBRARY}")
message(STATUS "${GPHOTO2_PORT_LIBRARY}")
message(STATUS "${GPHOTO2_VERSION_STRING}")
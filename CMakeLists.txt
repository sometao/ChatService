# CMakeList.txt : CMake project for FFmpegCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ChatService")

# --------------------------------------------------------------------------
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")  # deal with MSVC Warning C4530
ENDIF(MSVC)


######################################
#  WS2 lib setting (For windows)
######################################
find_library(WS2_LIBRARY ws2_32)


######################################
#  information
######################################

message(CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
message(PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})
message(WS2_LIBRARY=${WS2_LIBRARY})


############################################
# Add source to this project's executable.
############################################
add_executable (${PROJECT_NAME} 
	src/Launcher.cpp
	src/SocketClient.cpp
	src/SocketServer.cpp
	src/UserAgent.cpp
	src/socketUtil.cpp
)


target_include_directories( ${PROJECT_NAME}  
	PRIVATE 
		${PROJECT_SOURCE_DIR}/include
)

target_link_libraries( ${PROJECT_NAME}   
	PRIVATE 
		${WS2_LIBRARY} 
)




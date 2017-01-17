project(unprecacher)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(src SRC_LIST)

set(THIRD_PARTY "${CMAKE_SOURCE_DIR}/third_party")
set(MAIN_SOURCE "${CMAKE_SOURCE_DIR}/src")

set(METAMOD "${THIRD_PARTY}/metamod-hl1/metamod")
set(HLSDK "${THIRD_PARTY}/hlsdk")

set(API_INCLUDE_PATH "${HLSDK}/common ${HLSDK}/dlls ${HLSDK}/engine ${HLSDK}/pm_shared ${HLSDK}/game_shared ${HLSDK}/public ${METAMOD}")

string(REPLACE " " ";" API_INCLUDE_PATH_LIST ${API_INCLUDE_PATH})
include_directories(${API_INCLUDE_PATH_LIST})
foreach(ONE_PATH ${API_INCLUDE_PATH_LIST})
file(GLOB API_INCLUDE_FILES_TEMP ${ONE_PATH}/*.h)
set(API_INCLUDE_FILES "${API_INCLUDE_FILES};${API_INCLUDE_FILES_TEMP}")
endforeach()
add_subdirectory(test)
file(GLOB_RECURSE SOURCE_FILES ${MAIN_SOURCE}/*.h ${MAIN_SOURCE}/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32 -static-libstdc++")
#target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)

add_library(${PROJECT_NAME}_static STATIC ${SRC_LIST} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}_static PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

if (CMAKE_VERSION VERSION_LESS "3.1")
   if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
     set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
   endif ()
else ()
   set (CMAKE_CXX_STANDARD 11)
endif ()


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# -Wundef -Wlong-long -Wpadded
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -Wzero-as-null-pointer-constant -Weffc++ -Wunknown-pragmas")
	if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
		set(CMAKE_CXX_FLAGS "-O2 -finline-functions")
		set(CMAKE_SHARED_LINKER_FLAGS "-s")
	endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	 add_definitions(-D_DEBUG)
endif()



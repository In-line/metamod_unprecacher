project(unprecacher)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)

# src
set(THIRD_PARTY "${CMAKE_SOURCE_DIR}/third_party")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.h ${SRC_DIR}/*.cpp)

# api
set(METAMOD "${THIRD_PARTY}/metamod-hl1/metamod")
set(HLSDK "${THIRD_PARTY}/hlsdk")

set(API_INCLUDE_PATH "${HLSDK}/common ${HLSDK}/dlls ${HLSDK}/engine ${HLSDK}/pm_shared ${HLSDK}/game_shared ${HLSDK}/public ${METAMOD}")

string(REPLACE " " ";" API_INCLUDE_PATH_LIST ${API_INCLUDE_PATH})
include_directories(${API_INCLUDE_PATH_LIST})

# tests
add_library(${PROJECT_NAME}_static STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}_static PROPERTIES COMPILE_FLAGS "-m32 -O0" LINK_FLAGS "-m32 -O0")
add_subdirectory(test)

# main
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set(FLAGS "-m32 -O3 -pipe -fvisibility-inlines-hidden -fvisibility=hidden -flto -fPIC")
set_target_properties(
        ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS
        "${FLAGS} \
        -fdata-sections \
        -ffunction-sections \
        -Wall \
        -Wextra \
        -Werror \
        -Wstrict-aliasing \
        -Weffc++ \
        -Wunknown-pragmas"
)
set_target_properties(
        ${PROJECT_NAME} PROPERTIES LINK_FLAGS
        "${FLAGS} \
        -static-libgcc \
        -static-libstdc++ \
        -s \
        -Wl,--version-script=${CMAKE_SOURCE_DIR}/version_script.lds \
        -Wl,--gc-sections"
)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "_i386.so")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
else()
	message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	 add_definitions(-D_DEBUG)
endif()




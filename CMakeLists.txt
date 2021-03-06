﻿# CMakeList.txt : CMake project for test_NDI, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("test_NDI")
add_definitions(-std=c++17)
#SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#if (MSVC_VERSION GREATER_EQUAL "1900")
#    include(CheckCXXCompilerFlag)
#    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
#    if (_cpp_latest_flag_supported)
#        add_compile_options("/std:c++latest")
#    endif()
#endif()


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc/ffmpeg ${PROJECT_SOURCE_DIR}/inc/NDI)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(./test_NDI DIR_SRCS)


add_subdirectory(Lib.Base)
add_subdirectory(Lib.Config)
add_subdirectory(Lib.FrameConsumerNDI)
add_subdirectory(Lib.FrameProviderYuvFile)

# Add source to this project's executable.
add_executable (test_NDI ${DIR_SRCS})

target_link_libraries(test_NDI Lib.Base Lib.Config Lib.FrameConsumerNDI Lib.FrameProviderYuvFile)

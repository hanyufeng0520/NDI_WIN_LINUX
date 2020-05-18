# CMakeList.txt : CMake project for test_NDI, include source and define
# project specific logic here.

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_BASE_SRCS)
add_definitions(-std=c++17)
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

find_package(Threads)
link_libraries(${CMAKE_THREAD_LIBS_INIT})

# 生成链接库
add_library (Lib.Base ${DIR_LIB_BASE_SRCS})

# CMakeList.txt : CMake project for test_NDI, include source and define
# project specific logic here.

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_FrameConsumerNDI_SRCS)

set(OPENCV_DYNAMIC_LIBS  "${PROJECT_SOURCE_DIR}/lib")
add_library(libndi_core SHARED IMPORTED )
set_target_properties(libndi_core PROPERTIES   IMPORTED_LOCATION "${OPENCV_DYNAMIC_LIBS}/libndi.so.4.5.2")
set(mNDI_LIBS  libndi_core)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ")
link_libraries(${mNDI_LIBS} )

link_libraries(libavutil.so swresample.so)
link_libraries(Lib.Base)
# 生成链接库
add_library (Lib.FrameConsumerNDI ${DIR_LIB_FrameConsumerNDI_SRCS})

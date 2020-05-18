# CMakeList.txt : CMake project for test_NDI, include source and define
# project specific logic here.

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_FrameProviderYuvFile_SRCS)

# 生成链接库
add_library (Lib.FrameProviderYuvFile ${DIR_LIB_FrameProviderYuvFile_SRCS})

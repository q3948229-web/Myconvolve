cmake_minimum_required(VERSION 3.10)

# 2. 项目名称设为 "MatrixProcessor"
project(MatrixProcessor)

# 1. 使用 C++98 标准
# 6. 确保严格遵循 C++98 标准
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. 包含当前目录下的 include 文件夹
include_directories(${CMAKE_SOURCE_DIR}/include)

# 7. 包含必要的编译器警告选项
# 3. 针对 g++ 编译器设置选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 5. 生成可执行文件名为 "matrix_conv"
add_executable(matrix_conv src/main.cpp src/Convolution.cpp src/SobelDetector.cpp)
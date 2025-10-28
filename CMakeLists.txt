cmake_minimum_required(VERSION 3.10)  # 要求CMake版本至少3.10
project(DS2025 VERSION 1.0 LANGUAGES CXX)  # 项目名称：DS2025，版本1.0，语言C++

set(CMAKE_CXX_STANDARD 11)  # 使用C++11标准（兼容大部分编译器）
set(CMAKE_CXX_STANDARD_REQUIRED True)  # 强制要求C++11标准

# 1. 添加核心模块（src/下的代码）
add_library(core STATIC  # 生成静态库（core.lib），方便测试程序链接
    src/listnode.cpp
    src/stack.cpp
    src/vector.cpp
)

# 2. 添加测试程序（test/test.cpp）
add_executable(test_mystl test/test.cpp)  # 生成可执行文件test_mystl.exe
target_link_libraries(test_mystl core)  # 链接核心库（core.lib）

# 3. 添加实验1的程序（exp1/下的三个程序）
add_subdirectory(exp1)  # 进入exp1文件夹，执行其CMakeLists.txt
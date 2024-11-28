cmake_minimum_required(VERSION 3.28)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_PREFIX_PATH "E:/win_qt5/5.15.2/msvc2019_64/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
    # 对于 Visual Studio 编译器 执行源文件编译，和执行时为utf-8
    #/utf-8 选项 等价于 /source-charset:utf-8 + execution-charset:utf-8。
    message(STATUS "Using MSVC compiler")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif()

add_subdirectory(QSpyPlugins)
add_subdirectory(test)



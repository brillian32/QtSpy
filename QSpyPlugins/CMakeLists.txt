cmake_minimum_required(VERSION 3.28)
project(QSpyPlugins)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "E:/win_qt5/5.15.2/msvc2019_64/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE Debug)

if (MSVC)
    # 对于 Visual Studio 编译器 执行源文件编译，和执行时为utf-8
    #/utf-8 选项 等价于 /source-charset:utf-8 + execution-charset:utf-8。
    message(STATUS "Using MSVC compiler")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif()

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_library(QSpyPlugins MODULE
        ${CMAKE_CURRENT_SOURCE_DIR}/QSpy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QSpy.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IQSpyInterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DrawInfoWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DrawInfoWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ObjTreeWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ObjTreeWidget.cpp
)

target_link_libraries(QSpyPlugins
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()


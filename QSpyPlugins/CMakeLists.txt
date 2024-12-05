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

# 添加资源文件
set(RESOURCES
        res.qrc
)

# 使用 qt5_add_resources 将资源文件添加到项目中
# add_library已自动支持qrc转换 autoRCC
#qt5_add_resources(RESOURCE_FILES ${RESOURCES})

add_library(QSpyPlugins MODULE
        ${CMAKE_CURRENT_SOURCE_DIR}/QSpy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QSpy.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DrawInfoWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DrawInfoWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ObjTreeWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ObjTreeWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TreeObjMenu.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TreeObjMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TreeObjMenu.ui
        SwitchToolWidget.cpp
        SwitchToolWidget.h
        ${RESOURCES}
)

target_include_directories(QSpyPlugins
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

if (MSVC)
    target_link_libraries(QSpyPlugins
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
    )

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # 如果未安装，则执行安装 x11支持： libx11-dev libxtst-dev libxext-dev
    find_package(X11 QUIET)
    if(NOT X11_FOUND)
        message(STATUS "Required packages not found. Installing with apt-get...")
        execute_process(
                COMMAND sudo -S apt-get install -y libx11-dev libxtst-dev libxext-dev
                RESULT_VARIABLE APT_INSTALL_STATUS
        )
        if (APT_INSTALL_STATUS)
            message(FATAL_ERROR "Failed to install required libraries using apt-get!")
        endif ()
    endif ()
    target_link_libraries(QSpyPlugins
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
            X11
            Xtst
            Xext
    )
endif ()

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


# 完整的Qt项目CMake配置示例
# 文件名: complete-qt-project.cmake
cmake_minimum_required(VERSION 3.16)

# 项目信息
project(MyQtProject VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Qt 自动化处理 ===
set(CMAKE_AUTOMOC ON)          # 启用MOC
set(CMAKE_AUTORCC ON)          # 启用资源编译
set(CMAKE_AUTOUIC ON)          # 启用UI文件处理

# === Qt版本检测和配置 ===
# 尝试查找Qt5，如果失败则尝试Qt6
find_package(Qt5 QUIET COMPONENTS Core Network SerialPort Widgets)

if(NOT Qt5_FOUND)
    find_package(Qt6 REQUIRED COMPONENTS Core Network SerialPort Widgets)
    set(QT_VERSION_MAJOR 6)
else()
    set(QT_VERSION_MAJOR 5)
endif()

# === 编译选项 ===
if(MSVC)
    # Windows/MSVC配置
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)

    # MSVC特定的Qt配置
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
else()
    # GCC/Clang配置
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Wno-unused-parameter
    )
endif()

# === 调试和发布配置 ===
# 调试模式定义
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        DEBUG_MODE
        QT_QML_DEBUG  # 启用QML调试（如果使用）
    )
endif()

# === 预编译头文件优化 ===
option(ENABLE_PCH "Enable precompiled headers" ON)
if(ENABLE_PCH AND NOT CMAKE_VERSION VERSION_LESS "3.16")
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        <QtGlobal>
        <QObject>
        <QString>
        <QByteArray>
        <QDebug>
    )
endif()

# === 源文件收集 ===
# 自动收集所有源文件
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "src/*.ui"
    "src/*.qrc"
)

# === 可执行文件创建 ===
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# === Qt库链接 ===
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Network
        Qt6::SerialPort
        Qt6::Widgets
    )
else()
    target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Network
        Qt5::SerialPort
        Qt5::Widgets
    )
endif()

# === 包含目录配置 ===
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# === 平台特定配置 ===
if(WIN32)
    # Windows特定配置
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32  # Windows Socket库
    )

    # Windows subsystem配置
    if(NOT BUILD_GUI_APPLICATION)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE FALSE
        )
    endif()

elseif(UNIX AND NOT APPLE)
    # Linux配置
    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread  # POSIX线程库
    )
endif()

# === 交叉编译支持 ===
# 检测交叉编译环境
if(DEFINED ENV{SDK_ENVIRONMENT})
    message(STATUS "交叉编译环境检测到")

    # 设置交叉编译工具链（如果未在命令行中设置）
    if(NOT CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE $ENV{SDK_TOOLCHAIN_FILE} CACHE PATH "")
    endif()

    # 设置目标系统根目录
    if(DEFINED ENV{SDK_SYSROOT})
        set(CMAKE_SYSROOT $ENV{SDK_SYSROOT})
    endif()

    # 添加交叉编译特定的编译定义
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        CROSS_COMPILATION
    )
endif()

# === 驱动编译控制 ===
# 支持硬件驱动的条件编译
option(ENABLE_DRIVER_COMPILATION "启用硬件驱动编译" ON)

if(ENABLE_DRIVER_COMPILATION)
    message(STATUS "硬件驱动编译已启用")

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        HARDWARE_SUPPORT_ENABLED
    )

    # 添加驱动相关的源文件
    file(GLOB_RECURSE DRIVER_SOURCES
        "Driver/**/*.cpp"
        "Driver/**/*.h"
    )

    target_sources(${PROJECT_NAME} PRIVATE ${DRIVER_SOURCES})

    # 驱动相关的包含目录
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Driver
    )

    # 驱动依赖库检查
    find_library(METAL_LIBRARY metal)
    find_library(RFDC_LIBRARY rfdc)

    if(METAL_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${METAL_LIBRARY})
    endif()

    if(RFDC_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${RFDC_LIBRARY})
    endif()

else()
    message(STATUS "软件仿真模式已启用")

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        SOFTWARE_SIMULATION_MODE
    )
endif()

# === 安装配置 ===
# 可执行文件安装
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 配置文件安装（如果有）
if(EXISTS ${CMAKE_SOURCE_DIR}/config)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/config/
        DESTINATION etc/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.conf"
    )
endif()

# === 开发者工具 ===
# 添加自定义目标用于调试
add_custom_target(debug-build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Debug
    COMMENT "构建Debug版本"
)

add_custom_target(release-build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Release
    COMMENT "构建Release版本"
)

# 清理目标
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMENT "完全清理构建文件"
)

# === 测试支持 ===
# 启用测试（可选）
option(BUILD_TESTS "构建测试" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# === 文档生成 ===
# Doxygen支持（可选）
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in
                   ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "生成API文档" VERBATIM
    )
endif()

# === 信息输出 ===
message(STATUS "=== 项目配置信息 ===")
message(STATUS "项目名称: ${PROJECT_NAME}")
message(STATUS "版本: ${PROJECT_VERSION}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "Qt版本: ${QT_VERSION_MAJOR}")
message(STATUS "编译器: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "平台: ${CMAKE_SYSTEM_NAME}")
message(STATUS "架构: ${CMAKE_SYSTEM_PROCESSOR}")

if(ENABLE_DRIVER_COMPILATION)
    message(STATUS "驱动编译: 启用")
else()
    message(STATUS "驱动编译: 禁用 (软件仿真)")
endif()

message(STATUS "======================")
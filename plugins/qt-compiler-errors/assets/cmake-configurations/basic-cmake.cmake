cmake_minimum_required(VERSION 3.16)
project(MyQtProject VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用Qt的MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Network
    SerialPort
    Widgets  # 如果需要GUI
)

# 设置编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x050F00
)

# 添加子目录（如果有）
add_subdirectory(src)

# 收集源文件
file(GLOB_RECURSE SOURCES
    "*.cpp"
    "*.h"
    "*.hpp"
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Network
    Qt5::SerialPort
    Qt5::Widgets  # 如果需要GUI
)

# 包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 编译器特定设置
if(MSVC)
    # Windows/MSVC设置
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    # GCC/Clang设置
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 调试构建配置
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
endif()

# 跨编译设置（可选）
if(DEFINED ENV{SDK_ENVIRONMENT})
    # 设置交叉编译工具链
    set(CMAKE_TOOLCHAIN_FILE $ENV{SDK_TOOLCHAIN_FILE})
    set(CMAKE_SYSROOT $ENV{SDK_SYSROOT})
endif()
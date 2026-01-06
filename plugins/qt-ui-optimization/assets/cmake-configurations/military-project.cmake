#军工项目CMake配置 - 专为主题优化设计
#适用于军工、安防、监控类Qt应用

cmake_minimum_required(VERSION 3.16)
project(MilitaryQtApp VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 启用Qt的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置源文件
set(SOURCES
    main.cpp
    military_dashboard.cpp
)

# 设置头文件
set(HEADERS
    military_dashboard.h
)

# 设置资源文件
set(RESOURCES
    military_resources.qrc
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

# 设置编译器特定选项
if(MSVC)
    # Windows特定设置
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WIN32_WINNT=0x0601
        WIN32_LEAN_AND_MEAN
    )
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        WIN32_EXECUTABLE TRUE
    )
elseif(UNIX AND NOT APPLE)
    # Linux特定设置
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(APPLE)
    # macOS特定设置
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MACOSX_BUNDLE TRUE
    )
    set_source_files_properties(${RESOURCES} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
endif()

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# 军工主题特定配置说明
message(STATUS "军工项目配置完成:")
message(STATUS "  - 主题: Military Camouflage")
message(STATUS "  - 配色方案: 军绿、战术橙、HUD绿、橄榄褐、卡其色")
message(STATUS "  - 字体: Consolas、Monaco (等宽字体)")
message(STATUS "  - 专用组件: 雷达显示、战术按钮、HUD控件")
message(STATUS "  - 适用场景: 军工软件、安防监控、工业控制")

# 可选：添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
    message(STATUS "  - 构建模式: Debug (包含调试信息)")
else()
    message(STATUS "  - 构建模式: Release (优化构建)")
endif()

# 设置主题相关宏定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    MILITARY_THEME_ENABLED=1
    CAMOUFLAGE_COLORS=1
    HUD_STYLE_COMPONENTS=1
)
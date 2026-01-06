# 完整的Qt项目qmake配置示例
# 文件名: complete-qt-project.pro

# === 项目基本信息 ===
QT += core network serialport widgets

# 如果使用Qt6，取消注释以下行
# QT += core network serialport widgets

# 项目名称
TARGET = MyQtProject

# 项目模板（app = 应用程序, lib = 静态库, dll = 动态库）
TEMPLATE = app

# 项目版本
VERSION = 1.0.0

# === 配置选项 ===
CONFIG += c++17
CONFIG += warn_on
CONFIG += resources

# 如果需要Unicode支持
CONFIG += unicode

# 如果需要调试信息
CONFIG += debug_and_release

# === 构建配置 ===
# Release配置
CONFIG(release, debug|release) {
    CONFIG += release optimize_full
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
}

# Debug配置
CONFIG(debug, debug|release) {
    CONFIG += debug
    DEFINES += DEBUG_MODE
}

# === 编译器特定设置 ===
# MSVC配置
win32-msvc* {
    QMAKE_CXXFLAGS += /W4
    QMAKE_CXXFLAGS += /D_CRT_SECURE_NO_WARNINGS
    QMAKE_CXXFLAGS += /DWIN32_LEAN_AND_MEAN
    QMAKE_CXXFLAGS += /DNOMINMAX

    # 禁用一些MSVC警告
    QMAKE_CXXFLAGS -= /WX  # 禁用警告视为错误
}

# GCC/MinGW配置
gcc* {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-unused-variable
}

# Clang配置
clang* {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-unused-variable
}

# === 平台特定配置 ===
# Windows配置
win32 {
    # Windows子系统配置（控制台应用）
    # CONFIG += console
    # 或者GUI应用（默认）
    CONFIG += windows

    # Windows库链接
    LIBS += -lws2_32

    # Windows输出目录
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/../bin/debug
    } else {
        DESTDIR = $$PWD/../bin/release
    }
}

# Linux/Unix配置
unix {
    # POSIX线程库
    LIBS += -lpthread

    # 安装路径
    target.path = /usr/local/bin
    INSTALLS += target

    # 输出目录
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/../bin/debug
    } else {
        DESTDIR = $$PWD/../bin/release
    }
}

# macOS配置
macx {
    # macOS特定设置
    CONFIG += app_bundle
    QMAKE_INFO_PLIST = Info.plist
}

# === 驱动编译控制 ===
# 启用/禁用硬件驱动编译
ENABLE_DRIVERS = 1  # 设置为0禁用驱动编译

equals(ENABLE_DRIVERS, 1) {
    message("硬件驱动编译已启用")
    DEFINES += HARDWARE_SUPPORT_ENABLED

    # 驱动源文件目录
    DRIVER_PATH = $$PWD/Driver

    # 包含驱动头文件
    INCLUDEPATH += $$DRIVER_PATH/bsp
    INCLUDEPATH += $$DRIVER_PATH/lmk04828
    INCLUDEPATH += $$DRIVER_PATH/rf_adda
    INCLUDEPATH += $$DRIVER_PATH/reg
    INCLUDEPATH += $$DRIVER_PATH/microwave

    # 驱动源文件
    DRIVERSOURCES += \
        $$DRIVER_PATH/bsp/*.cpp \
        $$DRIVER_PATH/lmk04828/*.cpp \
        $$DRIVER_PATH/rf_adda/*.cpp \
        $$DRIVER_PATH/reg/*.cpp \
        $$DRIVER_PATH/microwave/*.cpp

    # 条件编译：如果驱动文件存在则包含
    exists($$DRIVER_PATH/bsp/SPI_Driver.cpp) {
        message("找到SPI驱动")
    } else {
        warning("未找到SPI驱动，使用模拟实现")
    }

} else {
    message("软件仿真模式已启用")
    DEFINES += SOFTWARE_SIMULATION_MODE
}

# === 源文件配置 ===
# 主源文件
SOURCES += \
    src/main.cpp

# 应用程序源文件
APP_SOURCES += \
    src/*.cpp

# 服务模块源文件
SERVER_SOURCES += \
    Server/rfdc_Interface/*.cpp \
    Server/recon_Iface/*.cpp

# 添加源文件到项目
SOURCES += $$APP_SOURCES $$SERVER_SOURCES

# 根据驱动编译设置添加驱动源文件
equals(ENABLE_DRIVERS, 1) {
    SOURCES += $$DRIVERSOURCES
}

# === 头文件配置 ===
HEADERS += \
    src/*.h \
    Server/rfdc_Interface/*.h \
    Server/recon_Iface/*.h

# 根据驱动编译设置添加驱动头文件
equals(ENABLE_DRIVERS, 1) {
    HEADERS += \
        $$DRIVER_PATH/bsp/*.h \
        $$DRIVER_PATH/lmk04828/*.h \
        $$DRIVER_PATH/rf_adda/*.h \
        $$DRIVER_PATH/reg/*.h \
        $$DRIVER_PATH/microwave/*.h
}

# === 资源文件配置 ===
# 如果有资源文件
RESOURCES += \
    resources/application.qrc

# UI文件（如果有）
FORMS += \
    ui/*.ui

# === 翻译文件 ===
# 如果需要国际化支持
TRANSLATIONS += \
    translations/app_zh_CN.ts \
    translations/app_en_US.ts

# === 库依赖 ===
# 系统库
unix:!macx {
    LIBS += -ldl
}

# 硬件相关库（仅在启用驱动时）
equals(ENABLE_DRIVERS, 1) {
    # 尝试链接Xilinx库（如果存在）
    LIBS += -lmetal -lrfdc

    # 检查库是否存在
    !exists(/usr/lib/libmetal.so) {
        warning("未找到libmetal库，可能影响硬件功能")
    }

    !exists(/usr/lib/librfdc.so) {
        warning("未找到librfdc库，可能影响RFSoC功能")
    }
}

# === 包含路径 ===
INCLUDEPATH += \
    . \
    src \
    Server \
    $$PWD/include

# 根据驱动编译设置添加驱动包含路径
equals(ENABLE_DRIVERS, 1) {
    INCLUDEPATH += $$DRIVER_PATH
}

# === 编译定义 ===
DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050F00

# 项目特定定义
DEFINES += \
    PROJECT_NAME=\\\"$$TARGET\\\" \
    PROJECT_VERSION=\\\"$$VERSION\\\"

# === 交叉编译支持 ===
# 检测交叉编译环境
cross_compile {
    message("交叉编译模式已启用")

    # 设置交叉编译工具链（如果需要）
    # QMAKE_CXX = aarch64-linux-gnu-g++
    # QMAKE_LINK = aarch64-linux-gnu-g++

    # 目标系统相关设置
    DEFINES += CROSS_COMPILATION

    # 设置目标根文件系统（如果需要）
    # SYSROOT = /path/to/sysroot
    # QMAKE_CXXFLAGS += --sysroot=$$SYSROOT
    # QMAKE_LFLAGS += --sysroot=$$SYSROOT
}

# === 自定义目标 ===
# 调试构建
debug_target.target = debug-build
debug_target.commands = $(MAKE) debug
debug_target.depends = first
QMAKE_EXTRA_TARGETS += debug_target

# 发布构建
release_target.target = release-build
release_target.commands = $(MAKE) release
release_target.depends = first
QMAKE_EXTRA_TARGETS += release_target

# 清理所有构建文件
clean_all_target.target = clean-all
clean_all_target.commands = $(MAKE) clean && rm -rf Makefile .qmake.stash *.o *.pro.user
clean_all_target.depends = first
QMAKE_EXTRA_TARGETS += clean_all_target

# 代码格式化（需要安装clang-format）
format_target.target = format
format_target.commands = find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i
QMAKE_EXTRA_TARGETS += format_target

# === 安装配置 ===
# 配置文件安装
config_files.path = /etc/$$TARGET
config_files.files = config/*.conf
INSTALLS += config_files

# 文档安装
documentation.path = /usr/share/doc/$$TARGET
documentation.files = README.md LICENSE
INSTALLS += documentation

# === 构建信息输出 ===
message("=== 项目配置信息 ===")
message("项目名称: $$TARGET")
message("项目版本: $$VERSION")
message("构建模式: $$CONFIG")
message("Qt版本: $$[QT_VERSION]")
message("编译器: $$[QMAKE_COMPILER]")
message("平台: $$[QMAKE_PLATFORM]")

equals(ENABLE_DRIVERS, 1) {
    message("驱动编译: 启用")
} else {
    message("驱动编译: 禁用 (软件仿真)")
}

message("输出目录: $$DESTDIR")
message("源文件数量: $$count(SOURCES)")
message("头文件数量: $$count(HEADERS)")
message("======================")
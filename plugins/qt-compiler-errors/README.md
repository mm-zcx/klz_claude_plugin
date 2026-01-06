# Qt编译错误处理技能

## 概述

这个技能提供了一套完整的Qt编译错误诊断和修复解决方案，基于实际项目中遇到的常见问题开发而成。

## 使用方法

### 自动修复脚本

运行自动化修复脚本来检测和修复常见的Qt编译错误：

```bash
# 在项目根目录运行
python3 .claude/skills/qt-compiler-errors/scripts/fix_qt_errors.py

# 指定特定项目路径
python3 .claude/skills/qt-compiler-errors/scripts/fix_qt_errors.py /path/to/qt/project
```

### 诊断流程

1. **识别错误类型**
   - MOC错误：检查Q_OBJECT宏和类继承
   - 链接错误：检查库依赖和缺失符号
   - 编译错误：检查语法和类型匹配

2. **应用快速修复**
   - 清理构建目录
   - 重新运行qmake/cmake
   - 重新构建项目

3. **处理持久问题**
   - 检查Qt版本兼容性
   - 验证模块链接
   - 确保正确的类结构

## 技能文件说明

### SKILL.md
主要技能文档，包含：
- 详细的错误分类和解决方案
- 诊断流程
- 最佳实践指南

### scripts/fix_qt_errors.py
自动修复脚本，功能包括：
- 检测并添加Q_OBJECT宏
- 修复过时的Qt信号名
- 添加Qt模块依赖
- 修复头文件保护

### references/qt_error_codes.md
详细的错误代码参考，包含：
- MOC错误详解
- 信号槽连接问题
- 构建系统配置错误
- 类型转换问题
- Qt版本兼容性

### assets/
项目模板文件：
- `cmake-template.txt`: 完整的CMakeLists.txt模板
- `pro-template.txt`: 完整的.pro文件模板

## 支持的错误类型

### MOC相关错误
- `undefined reference to 'vtable for ClassName'`
- `No rule to make target 'ClassName.moc'`
- Q_OBJECT宏缺失

### 信号槽错误
- 过时信号名（如`QAbstractSocket::error`）
- 参数类型不匹配
- 连接语法错误

### 构建系统错误
- 缺少Qt模块依赖
- 头文件路径问题
- CMake/qmake配置错误

### 类型转换错误
- quint16范围检查警告
- 未使用参数警告
- 类型转换问题

## 最佳实践建议

1. **项目结构**
   - 将类定义分离到头文件和源文件
   - 使用标准的项目组织结构

2. **构建管理**
   - 定期清理构建目录
   - 使用Qt Creator的内置构建工具
   - 正确配置项目依赖

3. **代码质量**
   - 始终为包含Q_OBJECT的类继承QObject
   - 使用Q_UNUSED宏抑制未使用参数警告
   - 保持Qt版本兼容性

## 更新日志

基于实际项目经验开发，包含WRJZH项目中遇到的实际问题和解决方案。

## 许可证

本技能遵循项目的开源许可证。
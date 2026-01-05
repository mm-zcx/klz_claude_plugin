# MyClaudeSkills Marketplace

这是一个用于 Claude Code 的个人插件市场，提供多个独立可安装的插件。

## 项目简介

本项目是一个 Claude Code 插件市场，包含多个独立的插件，您可以根据需要选择安装：

- **qt-compiler-errors** - Qt编译错误诊断和处理
- **qt-cpp-review** - Qt C++代码审查
- **qt-ui-optimization** - Qt UI优化
- **gitea** - Gitea集成和管理

## 市场安装

### 添加市场

```bash
# 在 Claude Code 中添加市场
/plugin marketplace add /path/to/MyClaudeSkills
```

或使用 Git 仓库：
```bash
/plugin marketplace add http://192.168.2.51:3000/mm-kzl/MyClaudeSkills.git
```

### 安装单个插件

```bash
# 安装 Qt 编译错误诊断插件
/plugin install qt-compiler-errors@myclaudeskills-marketplace

# 安装 Qt C++ 代码审查插件
/plugin install qt-cpp-review@myclaudeskills-marketplace

# 安装 Qt UI 优化插件
/plugin install qt-ui-optimization@myclaudeskills-marketplace

# 安装 Gitea 集成插件
/plugin install gitea@myclaudeskills-marketplace
```

## 项目结构

```
MyClaudeSkills/
├── .claude-plugin/
│   └── marketplace.json       # 市场清单文件
├── plugins/                    # 插件目录
│   ├── qt-compiler-errors/     # Qt编译错误插件
│   │   └── .claude-plugin/
│   │       └── plugin.json
│   ├── qt-cpp-review/          # Qt C++审查插件
│   │   └── .claude-plugin/
│   │       └── plugin.json
│   ├── qt-ui-optimization/     # Qt UI优化插件
│   │   └── .claude-plugin/
│   │       └── plugin.json
│   └── gitea/                  # Gitea集成插件
│       └── .claude-plugin/
│           └── plugin.json
├── README.md                   # 项目说明
├── INSTALL.md                  # 安装指南
├── EXAMPLES.md                 # 使用示例
└── CHANGELOG.md                # 版本变更日志
```

## 可用插件

### 1. qt-compiler-errors
- **版本**：1.0.0
- **功能**：Qt编译错误诊断和处理
- **覆盖**：CMake/qmake构建问题、MOC编译器问题、链接错误、信号槽错误等

### 2. qt-cpp-review
- **版本**：0.4.0
- **功能**：Qt C++代码审查
- **覆盖**：代码质量、规范性、性能优化、Qt特定规范检查

### 3. qt-ui-optimization
- **版本**：0.3.0
- **功能**：Qt UI优化
- **覆盖**：界面性能优化、绘制优化、布局优化

### 4. gitea
- **版本**：1.0.0
- **功能**：Gitea集成和管理
- **覆盖**：Issue管理、Pull Request、仓库管理、发布管理

## 插件管理

### 查看已安装插件

```bash
/plugin
```

### 启用/禁用插件

```bash
# 禁用插件
/plugin disable qt-compiler-errors@myclaudeskills-marketplace

# 启用插件
/plugin enable qt-compiler-errors@myclaudeskills-marketplace
```

### 卸载插件

```bash
/plugin uninstall qt-compiler-errors@myclaudeskills-marketplace
```

### 更新插件

```bash
# 卸载旧版本
/plugin uninstall qt-compiler-errors@myclaudeskills-marketplace

# 重新安装最新版本
/plugin install qt-compiler-errors@myclaudeskills-marketplace
```

## 开发信息

- **市场维护者**：mm-kzl
- **邮箱**：mm-kzl@outlook.com
- **市场版本**：1.0.0
- **Claude Code 要求**：≥ 1.0.0
- **创建时间**：2025-12-18

## 维护规则

- 每个插件独立维护和版本控制
- 所有文档使用中文编写
- 代码注释使用中文
- 定期更新和完善插件功能
- 保持项目结构清晰有序

---

*本项目持续维护中，欢迎定期查看更新内容。*

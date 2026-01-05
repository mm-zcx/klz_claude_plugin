# MyClaudeSkills Plugin

这是一个用于 Claude Code 的个人技能集合插件，以插件仓库形式维护。

## 项目简介

本项目是一个标准的 Claude Code 插件，包含多个实用的技能：

- **qt-compiler-errors** - Qt编译错误诊断和处理
- **qt-cpp-review** - Qt C++代码审查
- **qt-ui-optimization** - Qt UI优化
- **gitea** - Gitea集成和管理

## 插件安装

### 方式一：作为本地市场安装

```bash
# 在 Claude Code 中添加本地市场
/plugin marketplace add /path/to/MyClaudeSkills

# 安装插件
/plugin install myclaudeskills@myclaudeskills-marketplace
```

### 方式二：作为 Git 子模块使用

```bash
# 添加到父项目
git submodule add <仓库地址> MyClaudeSkills

# 更新子模块
git submodule update --remote MyClaudeSkills
```

## 插件结构

```
MyClaudeSkills/
├── .claude-plugin/
│   ├── plugin.json         # 插件清单文件
│   └── marketplace.json    # 市场清单文件
├── skills/                  # 技能目录
│   ├── qt-compiler-errors/
│   ├── qt-cpp-review/
│   ├── qt-ui-optimization/
│   └── gitea/
├── README.md               # 项目说明
├── CHANGELOG.md            # 版本变更日志
└── version-manager.py      # 版本管理工具
```

## 包含的技能

### 1. qt-compiler-errors
Qt编译错误诊断和处理技能，涵盖常见的CMake/qmake构建问题、MOC编译器问题、链接错误等。

### 2. qt-cpp-review
Qt C++代码审查技能，用于审查和分析Qt C++代码的质量、规范性和性能。

### 3. qt-ui-optimization
Qt UI优化技能，提供UI性能优化和用户体验改进建议。

### 4. gitea
Gitea集成技能，提供代码仓库管理、问题跟踪、Pull Request等功能。

## 版本管理

本项目遵循定制化的语义化版本控制规则：

- **主版本号**：重大架构变更或不兼容更新
- **次版本号**：新增技能时 +1
- **修订号**：技能内容更新时 +1

使用版本管理工具：
```bash
python version-manager.py status          # 查看当前版本
python version-manager.py add <技能名>    # 新增技能
python version-manager.py update <技能名> # 更新技能
```

## 开发信息

- **维护者**：mm-kzl
- **邮箱**：mm-kzl@outlook.com
- **插件版本**：1.0.0
- **Claude Code 要求**：≥ 1.0.0
- **创建时间**：2025-12-18

## 维护规则

- 以插件形式独立维护
- 所有文档使用中文编写
- 代码注释使用中文
- 定期更新和完善技能库内容
- 保持项目结构清晰有序

---

*本项目持续维护中，欢迎定期查看更新内容。*
# MyClaudeSkills 插件摘要

## 📦 插件信息

- **名称**：myclaudeskills
- **版本**：1.0.0
- **类型**：技能集合插件
- **维护者**：mm-kzl (mm-kzl@outlook.com)
- **Claude Code 要求**：≥ 1.0.0

## 🎯 插件简介

MyClaudeSkills 是一个为 Qt 开发者和 Gitea 用户设计的 Claude Code 插件，提供了四个核心技能来帮助提高开发效率：

1. **Qt 编译错误诊断** - 快速解决 Qt 编译问题
2. **Qt C++ 代码审查** - 确保代码质量和规范性
3. **Qt UI 优化** - 提升界面性能和用户体验
4. **Gitea 集成** - 无缝集成代码仓库管理

## 📁 插件结构

```
MyClaudeSkills/
├── .claude-plugin/              # 插件配置目录
│   ├── plugin.json             # 插件清单
│   └── marketplace.json        # 市场清单
├── skills/                      # 技能目录
│   ├── qt-compiler-errors/     # Qt 编译错误技能
│   ├── qt-cpp-review/          # Qt C++ 审查技能
│   ├── qt-ui-optimization/     # Qt UI 优化技能
│   └── gitea/                  # Gitea 集成技能
├── README.md                   # 项目说明
├── INSTALL.md                  # 安装指南
├── EXAMPLES.md                 # 使用示例
├── CHANGELOG.md                # 变更日志
├── CLAUDE.md                   # Claude 项目指南
├── version-manager.py          # 版本管理工具
└── verify-plugin.sh            # 插件验证脚本
```

## ✨ 核心功能

### 1. Qt 编译错误诊断技能

**用途**：快速诊断和解决 Qt 编译问题

**覆盖范围**：
- CMake/qmake 构建问题
- MOC (Meta-Object Compiler) 错误
- 信号槽连接问题
- 链接错误和 vtable 问题
- 跨线程通信问题

**使用场景**：遇到 Qt 编译错误时，直接描述错误信息即可

### 2. Qt C++ 代码审查技能

**用途**：全面审查 Qt C++ 代码质量

**审查维度**：
- Qt 特定规范（信号槽、对象树、容器）
- C++ 最佳实践（内存管理、RAII、const 正确性）
- 性能优化（避免拷贝、隐式共享）
- 代码风格一致性

**使用场景**：代码审查、规范检查、架构分析

### 3. Qt UI 优化技能

**用途**：优化 Qt 界面性能和用户体验

**优化方向**：
- 绘制性能优化
- 布局优化
- 数据加载优化
- 自定义控件优化

**使用场景**：界面卡顿、响应慢、内存占用高

### 4. Gitea 集成技能

**用途**：无缝集成 Gitea 代码管理功能

**支持功能**：
- Issue 管理和创建
- Pull Request 管理和审查
- 仓库信息查询
- 发布管理
- 通知管理

**使用场景**：代码仓库管理、团队协作

## 🚀 快速开始

### 安装步骤

1. **添加本地市场**
   ```bash
   /plugin marketplace add /path/to/MyClaudeSkills
   ```

2. **安装插件**
   ```bash
   /plugin install myclaudeskills@myclaudeskills-marketplace
   ```

3. **重启 Claude Code**

4. **验证安装**
   ```bash
   /help
   ```

### 验证插件

运行验证脚本：
```bash
./verify-plugin.sh
```

## 📖 文档

- **[README.md](README.md)** - 项目概述和基本信息
- **[INSTALL.md](INSTALL.md)** - 详细安装指南
- **[EXAMPLES.md](EXAMPLES.md)** - 使用示例和场景
- **[CHANGELOG.md](CHANGELOG.md)** - 版本变更历史
- **[CLAUDE.md](CLAUDE.md)** - Claude 项目配置指南

## 🔧 开发工具

### 版本管理

使用 `version-manager.py` 管理版本：

```bash
# 查看当前版本
python version-manager.py status

# 新增技能（次版本号 +1）
python version-manager.py add <技能名>

# 更新技能（修订号 +1）
python version-manager.py update <技能名> <旧版> <新版>

# 主版本更新（需要确认）
python version-manager.py major <新主版本> <描述>
```

### 插件验证

运行验证脚本检查插件配置：
```bash
./verify-plugin.sh
```

## 🎨 技能特性

### 技能自动激活

所有技能都配置了描述性的 name 和 description，Claude Code 会根据上下文自动激活相应的技能：

- 遇到 Qt 编译错误 → `qt-compiler-errors` 技能激活
- 请求代码审查 → `qt-cpp-review` 技能激活
- 讨论 UI 优化 → `qt-ui-optimization` 技能激活
- 提及 Gitea 操作 → `gitea` 技能激活

### 技能独立性

每个技能都是独立的，可以：
- 单独使用
- 组合使用
- 并行激活

## 🔄 版本管理规则

本项目遵循定制化的语义化版本控制：

- **主版本号 (Major)**：重大架构变更或不兼容更新（需用户确认）
- **次版本号 (Minor)**：新增技能时 +1，修订号重置为 0
- **修订号 (Patch)**：技能内容更新时 +1

### 示例

- 新增技能：`v0.0.1` → `v0.1.0`
- 更新技能：`v0.1.0` → `v0.1.1`
- 主版本更新：`v0.1.1` → `v1.0.0`（创建归档分支）

## 🛠️ 维护规则

- ✅ 所有文档使用中文编写
- ✅ 代码注释使用中文
- ✅ 保持项目结构清晰有序
- ✅ 遵循插件最佳实践
- ✅ 定期更新和完善内容

## 📊 插件统计

- **技能数量**：4
- **代码示例**：20+
- **文档数量**：8
- **支持平台**：macOS, Linux, Windows

## 🔗 相关链接

- **Claude Code 文档**：https://code.claude.com/docs
- **插件开发指南**：https://code.claude.com/docs/plugins
- **技能开发指南**：https://code.claude.com/docs/skills

## 💡 使用技巧

1. **提供上下文**：描述 Qt 版本、项目类型、具体问题
2. **明确需求**：指定关注的方面（性能、规范、安全）
3. **迭代改进**：根据建议进行修改并再次验证
4. **文档记录**：请求添加中文注释和使用文档

## 📞 获取支持

如有问题或建议，请联系：

- **维护者**：mm-kzl
- **邮箱**：mm-kzl@outlook.com
- **项目路径**：/Volumes/MACOS/Code/vscode/MyClaudeSkills

## 📝 许可证

本项目作为个人技能库维护，仅供学习和个人使用。

---

**插件版本**：1.0.0
**最后更新**：2026-01-05
**状态**：✅ 验证通过，可以安装使用

# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

这是一个以git子模块形式维护的Claude Skills库项目，用于存放和维护个人Claude技能相关的内容。

## 项目结构

- `README.md` - 项目说明文档，包含项目简介和使用方式
- `skills/` - 存放各种技能文件
- `docs/` - 技能相关文档
- `config/` - 配置文件
- `examples/` - 使用示例

## 维护规则

- 所有文档均使用中文编写
- 代码注释使用中文
- 以git子模块形式独立维护
- 保持项目结构清晰有序

## Git操作

该项目作为子模块在父项目中使用：

- 添加到父项目：`git submodule add <仓库地址> MyClaudeSkills`
- 更新子模块：`git submodule update --remote MyClaudeSkills`

## 开发信息

- 维护者：mm-kzl
- 邮箱：mm-kzl@outlook.com


- 每一次更新完成后需要增加子模块版本号和版本和子模块说明,并记录在版本日志中

## 版本管理规则

本项目遵循定制化的语义化版本控制规则：

### 版本号规则
- **主版本号**：重大架构变更或不兼容更新（由用户决定是否更新）
- **次版本号**：新增子模块时 +1
- **修订号**：子模块内容更新时 +1

### 项目记忆规则
- **子模块添加**：次版本号自动递增，修订号重置为0
- **子模块更新**：修订号自动递增
- **主版本更新**：需要用户确认，创建归档分支

### 版本管理流程
1. **主线开发**：main分支继续日常开发和迭代
2. **主版本归档**：当主版本号更新时，需要创建git分支进行归档
3. **分支命名**：归档分支命名为 `vX.X.x-archive`
4. **主线继续**：主线继续下一个版本的开发

### 版本操作示例
- 新增子模块：`v0.0.1` → `v0.1.0`
- 子模块更新：`v0.1.0` → `v0.1.1`
- 主版本更新：`v0.1.1` → `v1.0.0`（创建分支 `v0.x.x-archive`）

### 版本管理工具
使用 `version-manager.py` 脚本进行版本管理：
- `python version-manager.py status` - 查看当前版本状态
- `python version-manager.py add <子模块名> [版本]` - 新增子模块
- `python version-manager.py update <子模块名> <旧版> <新版>` - 更新子模块
- `python version-manager.py major <新主版本> <描述>` - 主版本更新
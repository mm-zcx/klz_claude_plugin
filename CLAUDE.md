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
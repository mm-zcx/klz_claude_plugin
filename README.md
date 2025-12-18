# MyClaudeSkills

这是一个用于存放和维护个人Claude Skills库的项目。

## 项目简介

本项目作为git子模块的形式维护，专门用于管理和维护Claude技能相关的内容，包括：

- 自定义Claude技能的开发和测试
- 技能配置文件的维护
- 技能文档和使用说明
- 技能优化和改进记录
- 与Claude Agent SDK集成的各种工具和功能

## 使用方式

本项目以git子模块的形式集成到主项目中，使用以下命令添加到父项目：

```bash
git submodule add <仓库地址> MyClaudeSkills
```

## 项目结构

```
MyClaudeSkills/
├── README.md          # 项目说明文档
├── skills/            # 存放各种技能文件
├── docs/              # 技能相关文档
├── config/            # 配置文件
└── examples/          # 使用示例
```

## 维护说明

- 以git子模块形式独立维护
- 所有文档均使用中文编写
- 代码注释使用中文
- 定期更新和完善技能库内容
- 保持项目结构清晰有序

## 更新子模块

在父项目中更新子模块：
```bash
git submodule update --remote MyClaudeSkills
```

## 开发信息

- 维护者：mm-kzl
- 邮箱：mm-kzl@outlook.com
- 创建时间：2025-12-18

---

*本项目持续维护中，欢迎定期查看更新内容。*
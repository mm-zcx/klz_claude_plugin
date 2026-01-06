# Claude Code Plugin Marketplace

个人 Claude Code 插件市场，提供 Qt 开发和 Gitea 相关的独立插件。

## 可用插件

- **qt-compiler-errors** - Qt编译错误诊断
- **qt-cpp-review** - Qt C++代码审查
- **qt-ui-optimization** - Qt UI优化
- **gitea** - Gitea集成

## 快速开始

```bash
# 添加市场
/plugin marketplace add https://github.com/mm-zcx/klz_claude_plugin.git

# 安装插件
/plugin install qt-compiler-errors@myclaudeskills-marketplace
/plugin install qt-cpp-review@myclaudeskills-marketplace
/plugin install qt-ui-optimization@myclaudeskills-marketplace
/plugin install gitea@myclaudeskills-marketplace
```

## 项目结构

```
klz_claude_plugin/
├── .claude-plugin/
│   └── marketplace.json       # 市场配置
├── plugins/                    # 独立插件
│   ├── qt-compiler-errors/
│   ├── qt-cpp-review/
│   ├── qt-ui-optimization/
│   └── gitea/
├── README.md
└── verify-marketplace.sh
```

## 验证安装

```bash
./verify-marketplace.sh
```

## 仓库地址

- **GitHub**: https://github.com/mm-zcx/klz_claude_plugin

---

**维护者**: mm-zcx

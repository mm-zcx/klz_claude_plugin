# MyClaudeSkills 插件安装指南

本指南将帮助您安装和配置 MyClaudeSkills 插件。

## 前置要求

- Claude Code 已安装并正常运行
- 对命令行操作有基本了解

## 安装方式

### 方式一：本地市场安装（推荐）

这是最快的安装方式，适合本地开发和测试。

1. **打开 Claude Code**
   ```bash
   claude
   ```

2. **添加本地市场**
   ```bash
   /plugin marketplace add /Volumes/MACOS/Code/vscode/MyClaudeSkills
   ```

3. **安装插件**
   ```bash
   /plugin install myclaudeskills@myclaudeskills-marketplace
   ```

4. **重启 Claude Code**
   安装完成后，需要重启 Claude Code 以加载插件。

5. **验证安装**
   重启后，运行以下命令查看可用技能：
   ```bash
   /help
   ```

### 方式二：Git 仓库安装

如果您将此仓库托管在 Git 服务器上：

1. **克隆仓库**
   ```bash
   git clone <your-repo-url> MyClaudeSkills
   ```

2. **添加为本地市场**
   ```bash
   /plugin marketplace add /path/to/MyClaudeSkills
   ```

3. **安装插件**
   ```bash
   /plugin install myclaudeskills@myclaudeskills-marketplace
   ```

### 方式三：作为子模块使用（推荐用于团队项目）

1. **在父项目中添加子模块**
   ```bash
   git submodule add <your-repo-url> MyClaudeSkills
   ```

2. **配置项目自动安装**

   在项目的 `.claude/settings.json` 中添加：
   ```json
   {
     "pluginMarketplaces": [
       {
         "name": "myclaudeskills-marketplace",
         "source": "./MyClaudeSkills"
       }
     ],
     "plugins": [
       {
         "name": "myclaudeskills",
         "marketplace": "myclaudeskills-marketplace",
         "enabled": true
       }
     ]
   }
   ```

3. **团队成员信任项目文件夹后，插件将自动安装**

## 使用技能

安装完成后，您可以直接使用插件中的技能。例如：

- Qt 编译错误诊断：直接描述遇到的 Qt 编译错误
- Qt C++ 代码审查：请求审查 Qt C++ 代码
- Qt UI 优化：询问 UI 优化建议
- Gitea 集成：使用 Gitea 相关功能

技能会自动激活，无需手动调用。

## 更新插件

### 更新本地市场

如果您对插件进行了修改：

```bash
# 卸载当前版本
/plugin uninstall myclaudeskills@myclaudeskills-marketplace

# 重新安装
/plugin install myclaudeskills@myclaudeskills-marketplace
```

### 更新 Git 子模块

```bash
# 在父项目中更新子模块
git submodule update --remote MyClaudeSkills
```

## 插件内容

本插件包含以下技能：

### 1. qt-compiler-errors
- **功能**：Qt 编译错误诊断和处理
- **使用场景**：遇到 CMake/qmake 构建问题、MOC 错误、链接错误等

### 2. qt-cpp-review
- **功能**：Qt C++ 代码审查
- **使用场景**：需要审查 Qt C++ 代码质量、规范性和性能

### 3. qt-ui-optimization
- **功能**：Qt UI 优化
- **使用场景**：需要改进 UI 性能和用户体验

### 4. gitea
- **功能**：Gitea 集成和管理
- **使用场景**：代码仓库管理、问题跟踪、Pull Request 等

## 卸载插件

如果需要卸载插件：

```bash
/plugin uninstall myclaudeskills@myclaudeskills-marketplace
```

## 故障排除

### 问题：插件安装后技能不可用

**解决方案**：
1. 确认已重启 Claude Code
2. 检查 `/help` 命令输出中是否包含相关技能
3. 查看插件安装日志

### 问题：技能无法自动激活

**解决方案**：
1. 确认技能文件中有正确的 front matter（name 和 description）
2. 检查技能文件是否位于正确的目录（`skills/<skill-name>/`）
3. 重新安装插件

### 问题：市场添加失败

**解决方案**：
1. 确认路径正确
2. 检查 `.claude-plugin/marketplace.json` 文件是否存在
3. 查看 Claude Code 日志了解详细错误信息

## 获取帮助

如有问题或建议，请联系：

- **维护者**：mm-kzl
- **邮箱**：mm-kzl@outlook.com

---

祝您使用愉快！

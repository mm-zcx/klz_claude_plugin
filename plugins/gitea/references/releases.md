# Gitea 发布版本管理

## 列出发布版本

### 列出当前仓库的所有发布版本
```bash
tea release list
```

**选项：**
- `--page`：页码
- `--limit`：每页数量限制

**示例：**
```bash
# 列出所有发布版本
tea release list

# 列出前 20 个发布版本
tea release list --limit 20
```

### 在其他仓库中列出发布版本
```bash
tea release list --repo owner/repo
```

## 查看发布版本

### 查看特定发布版本详情
```bash
tea release <发布版本标签或编号>
```

**示例：**
```bash
# 查看标签为 v1.0.0 的发布版本
tea release v1.0.0

# 查看编号为 1 的发布版本
tea release 1
```

## 创建发布版本

### 创建新发布版本
```bash
tea release create --tag <标签> --title "标题" --description "描述"
```

**选项：**
- `--tag`：Git 标签（必需）
- `--title`：发布标题（必需）
- `--description` 或 `--desc`：发布说明
- `--name`：发布名称（如果不同于标题）
- `--target`：目标提交哈希或分支名（默认为默认分支）
- `--draft`：创建为草稿（默认为 false）
- `--prerelease`：标记为预发布版本
- `--repo`：指定仓库（默认为当前仓库）

**示例：**
```bash
# 创建基本发布版本
tea release create --tag v1.0.0 --title "Version 1.0.0" --description "First stable release"

# 创建预发布版本
tea release create --tag v2.0.0-beta.1 --title "Beta Release" --prerelease

# 创建草稿发布版本
tea release create --tag v1.1.0 --title "Work in progress" --draft

# 创建指向特定提交的发布版本
tea release create --tag v1.0.1 --title "Hotfix" --target abc123

# 从文件创建发布说明
tea release create --tag v1.0.0 --title "Release" --description "$(cat release-notes.md)"
```

### 创建带附件的发布版本

tea 本身不支持直接上传附件，但可以通过以下方式：

1. **使用 Web 界面**：创建发布版本后，通过 Web 界面上传附件
2. **使用 Gitea API**：通过 API 上传附件到已创建的发布版本

## 编辑发布版本

### 编辑发布版本
```bash
tea release edit <发布版本标签> --title "新标题" --description "新描述"
```

**选项：**
- `--title`：新标题
- `--description` 或 `--desc`：新描述内容
- `--draft`：标记或取消标记为草稿
- `--prerelease`：标记或取消标记为预发布

**示例：**
```bash
# 更新标题
tea release edit v1.0.0 --title "Updated title"

# 将草稿发布
tea release edit v1.0.0 --draft=false

# 更新描述
tea release edit v1.0.0 --description "$(cat updated-notes.md)"

# 标记为预发布
tea release edit v2.0.0-beta.1 --prerelease
```

## 删除发布版本

### 删除发布版本
```bash
tea release delete <发布版本标签>
```

**示例：**
```bash
# 删除发布版本 v1.0.0
tea release delete v1.0.0
```

**注意**：这只会删除发布版本，不会删除 Git 标签。

## 发布版本工作流

### 标准发布流程
```bash
# 1. 确保所有更改已合并
git checkout main
git pull

# 2. 创建并推送 Git 标签
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# 3. 创建发布版本
tea release create --tag v1.0.0 --title "Version 1.0.0" --description "Release notes here"
```

### 使用草稿的工作流
```bash
# 1. 创建 Git 标签
git tag -a v1.1.0 -m "Version 1.1.0"
git push origin v1.1.0

# 2. 创建草稿发布版本
tea release create --tag v1.1.0 --title "Version 1.1.0" --draft

# 3. 通过 Web 界面上传附件并完善发布说明

# 4. 发布草稿
tea release edit v1.1.0 --draft=false
```

## 发布说明模板

### 基本发布说明模板
```markdown
# Version X.Y.Z

## 新增功能
- 新增功能 A
- 新增功能 B

## 改进
- 改进 X 的性能
- 优化 Y 的用户体验

## Bug 修复
- 修复问题 A
- 修复问题 B

## 升级说明
- 升级前请注意...

## 已知问题
- 暂无
```

### 预发布版本模板
```markdown
# Version X.Y.Z - Pre-release

**这是一个预发布版本，不建议在生产环境使用。**

## 测试重点
- 请重点测试...
- 反馈问题请创建 Issue...

## 更改内容
- 更改 A
- 更改 B
```

## 最佳实践

1. **语义化版本**：遵循语义化版本规范（如 v1.0.0, v2.1.3）
2. **Git 标签**：确保 Git 标签已推送到远程
3. **发布说明**：提供清晰、详细的发布说明
4. **草稿使用**：对于复杂发布，先创建草稿再发布
5. **预发布标记**：正确使用预发布标记（alpha, beta, rc）
6. **附件管理**：通过 Web 界面上传编译后的二进制文件

## 常用场景

### 稳定版本发布
```bash
git tag -a v1.0.0 -m "Stable release 1.0.0"
git push origin v1.0.0
tea release create --tag v1.0.0 --title "Version 1.0.0" --description "$(cat release-notes.md)"
```

### 热修复发布
```bash
git tag -a v1.0.1 -m "Hotfix release"
git push origin v1.0.1
tea release create --tag v1.0.1 --title "Hotfix v1.0.1" --description "Critical bug fixes"
```

### Beta 版本发布
```bash
git tag -a v2.0.0-beta.1 -m "Beta release"
git push origin v2.0.0-beta.1
tea release create --tag v2.0.0-beta.1 --title "Beta 1" --prerelease --description "Testing release"
```

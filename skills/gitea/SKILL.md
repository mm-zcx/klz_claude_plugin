---
name: gitea
description: Gitea 命令行工具（tea）使用指南，用于管理 Gitea 仓库、Issue、Pull Request、发布版本、标签、里程碑和通知。在以下场景使用：(1) 创建、克隆或查看 Gitea 仓库 (2) 管理 Issue（创建、编辑、评论、列表） (3) 管理 Pull Request（创建、审查、合并、checkout） (4) 创建和管理发布版本 (5) 管理标签和里程碑 (6) 查看和管理通知 (7) 在浏览器中快速打开仓库资源。不涉及组织管理功能。
---

# Gitea 命令行工具使用指南

## 概述

本技能提供完整的 Gitea 命令行工具（tea）使用指南，帮助你高效管理 Gitea 仓库和协作流程。tea 是 Gitea 的官方 CLI 工具，支持在终端中执行大部分 Gitea 操作。

**前提条件：**
- 已安装 tea CLI 工具
- 已通过 `tea login add` 登录到 Gitea 服务器
- 本地 git 仓库配置正确（tea 会自动检测当前目录的 git 配置）

## 核心功能速查

### 仓库管理
- 创建仓库：`tea repo create`
- 克隆仓库：`tea clone <url>`
- 查看仓库：`tea repo`
- 分支管理：`tea branch`
- 在浏览器打开：`tea open`

### Issue 管理
- 列出 Issue：`tea issue list`
- 创建 Issue：`tea issue create --title "标题"`
- 查看 Issue：`tea issue <编号>`
- 编辑 Issue：`tea issue edit <编号> --state closed`
- 添加评论：`tea comment <编号> --body "评论"`

### Pull Request 管理
- 列出 PR：`tea pr list`
- 创建 PR：`tea pr create --title "标题"`
- Checkout PR：`tea pr checkout <编号>`
- 合并 PR：`tea pr merge <编号>`
- 编辑 PR：`tea pr edit <编号> --state closed`

### 发布版本管理
- 列出发布：`tea release list`
- 创建发布：`tea release create --tag v1.0.0 --title "版本标题"`
- 编辑发布：`tea release edit v1.0.0 --draft=false`
- 删除发布：`tea release delete v1.0.0`

### 标签和里程碑管理
- 创建标签：`tea labels create --name bug --color "#d73a4a"`
- 创建里程碑：`tea milestones create --title "v1.0.0" --deadline 2026-03-31`
- 为 Issue 分配：`tea issue create --milestone "v1.0.0" --labels bug`

### 通知和辅助功能
- 查看通知：`tea notification list`
- 标记已读：`tea notification read <编号>`
- 查看用户：`tea whoami`
- 登录管理：`tea login add`

## 使用指南

### 仓库管理

详见 [repositories.md](repositories.md)

**常见操作：**
```bash
# 创建新仓库
tea repo create --name my-project --description "My new project"

# 克隆仓库
tea clone https://gitea.com/owner/repo.git

# 在浏览器中打开当前仓库
tea open

# 查看仓库详情
tea repo
```

**何时使用：**
- 需要创建新的 Gitea 仓库
- 需要克隆仓库到本地
- 需要查看仓库信息或分支
- 需要在浏览器中快速打开仓库

### Issue 管理

详见 [issues.md](issues.md)

**常见操作：**
```bash
# 创建 Issue
tea issue create --title "Bug found" --body "Description here" --labels bug

# 列出所有开放的 Issue
tea issue list --state open

# 查看 Issue 详情
tea issue 42

# 关闭 Issue
tea issue edit 42 --state closed

# 添加评论
tea comment 42 --body "Working on this"
```

**何时使用：**
- 需要创建、查看或编辑 Issue
- 需要按标签或里程碑筛选 Issue
- 需要为 Issue 添加评论
- 需要批量管理 Issue

### Pull Request 管理

详见 [pull-requests.md](pull-requests.md)

**常见操作：**
```bash
# 从当前分支创建 PR
tea pr create --title "Add feature" --description "PR description"

# 列出开放的 PR
tea pr list --state open

# Checkout PR 到本地进行审查
tea pr checkout 42

# 合并 PR
tea pr merge 42 --squash --delete-branch

# 查看特定 PR
tea pr 42
```

**何时使用：**
- 需要创建 Pull Request
- 需要审查他人的代码（checkout PR）
- 需要合并 PR
- 需要查看 PR 状态或添加评论

### 发布版本管理

详见 [releases.md](releases.md)

**常见操作：**
```bash
# 创建发布版本
git tag -a v1.0.0 -m "Release 1.0.0"
git push origin v1.0.0
tea release create --tag v1.0.0 --title "Version 1.0.0" --description "Release notes"

# 创建预发布版本
tea release create --tag v2.0.0-beta.1 --prerelease --title "Beta 1"

# 将草稿发布
tea release edit v1.1.0 --draft=false

# 列出所有发布版本
tea release list
```

**何时使用：**
- 需要创建新的发布版本
- 需要编辑发布版本信息
- 需要发布草稿版本
- 需要删除发布版本

### 标签和里程碑管理

详见 [labels-milestones.md](labels-milestones.md)

**常见操作：**
```bash
# 创建标签
tea labels create --name bug --description "Bug report" --color "#d73a4a"

# 创建里程碑
tea milestones create --title "v1.0.0" --deadline "2026-03-31"

# 为 Issue 分配标签和里程碑
tea issue create --title "Feature" --labels enhancement --milestone "v1.0.0"

# 查看特定里程碑的 Issue
tea issue list --milestone "v1.0.0"
```

**何时使用：**
- 需要设置项目标签体系
- 需要创建和管理里程碑
- 需要为 Issue 或 PR 分配标签/里程碑
- 需要按标签或里程碑筛选

### 通知和辅助功能

详见 [notifications.md](notifications.md)

**常见操作：**
```bash
# 查看未读通知
tea notification list --states unread

# 标记通知为已读
tea notification read 1

# 置顶重要通知
tea notification pin 1

# 在浏览器中打开 Issues 页面
tea open issues

# 查看当前登录用户
tea whoami
```

**何时使用：**
- 需要查看和管理通知
- 需要在浏览器中快速打开特定页面
- 需要管理多个 Gitea 账户
- 需要查看当前用户信息

## 工作流示例

### 标准开发流程
```bash
# 1. 创建仓库
tea repo create --name my-project --description "My new project"

# 2. 克隆到本地
tea clone https://gitea.com/owner/my-project.git
cd my-project

# 3. 开发功能
git checkout -b feature/new-feature
# ... 编写代码 ...
git add .
git commit -m "Add new feature"
git push -u origin feature/new-feature

# 4. 创建 PR
tea pr create --title "Add new feature" --description "Description"

# 5. 审查和合并
tea pr checkout 42  # 审查者
tea pr merge 42 --squash  # 合并者

# 6. 创建发布版本
git checkout main
git pull
git tag -a v1.0.0 -m "Release 1.0.0"
git push origin v1.0.0
tea release create --tag v1.0.0 --title "Version 1.0.0"
```

### Issue 管理流程
```bash
# 1. 设置标签和里程碑
tea labels create --name bug --color "#d73a4a"
tea labels create --name enhancement --color "#a2eeef"
tea milestones create --title "v1.0.0" --deadline "2026-03-31"

# 2. 创建 Issue
tea issue create \
  --title "Fix login bug" \
  --body "Login fails on Chrome" \
  --labels bug,high-priority \
  --milestone "v1.0.0"

# 3. 跟踪进度
tea issue list --milestone "v1.0.0" --state all

# 4. 更新状态
tea issue edit 42 --labels "in-progress"
tea issue edit 42 --state closed
```

## 最佳实践

### 1. Git 状态管理
tea 假设本地 git 状态已在远程发布。执行 tea 操作前，先提交并推送本地更改：
```bash
git add .
git commit -m "Commit message"
git push
tea pr create --title "PR title"
```

### 2. 上下文感知
tea 会自动使用当前目录（$PWD）的仓库上下文。在仓库目录中执行命令可省略 `--repo` 参数。

### 3. 登录管理
对于多个 Gitea 实例，使用命名登录：
```bash
tea login add --name work --url https://work-gitea.com
tea login add --name personal --url https://gitea.com
tea issue list --login work
```

### 4. 标签体系标准化
为项目建立一致的标签体系（参考 [labels-milestones.md](labels-milestones.md) 中的"常用标签建议"）。

### 5. 里程碑规划
使用里程碑规划版本发布，合理设置截止日期和 Issue 数量。

### 6. 发布版本工作流
- 先创建并推送 Git 标签
- 再创建发布版本
- 对于复杂发布，先创建草稿再完善

## 参考文档

本技能包含详细的参考文档，在需要时加载：

- **[repositories.md](repositories.md)**：仓库创建、克隆、查看、分支管理、浏览器集成
- **[issues.md](issues.md)**：Issue 创建、编辑、评论、列表、筛选、批量操作
- **[pull-requests.md](pull-requests.md)**：PR 创建、审查、合并、checkout、工作流
- **[releases.md](releases.md)**：发布版本创建、编辑、删除、工作流、模板
- **[labels-milestones.md](labels-milestones.md)**：标签和里程碑的创建、管理、最佳实践
- **[notifications.md](notifications.md)**：通知管理、浏览器集成、登录管理、多实例

**加载参考文档的时机：**
- 需要特定功能的详细参数时（如 `tea issue create` 的所有选项）
- 需要工作流指导时（如标准 PR 流程、发布版本流程）
- 需要最佳实践建议时（如标签体系设计、里程碑规划）

## 故障排查

### 命令未找到
确保 tea 已安装并在 PATH 中：
```bash
which tea
tea --version
```

### 未登录错误
执行登录命令：
```bash
tea login add
```

### 仓库上下文错误
确保在正确的 git 仓库目录中，或使用 `--repo` 参数指定仓库：
```bash
cd /path/to/repo
tea issue list

# 或
tea issue list --repo owner/repo
```

### 权限错误
确保当前登录用户对目标仓库有相应权限。使用 `tea whoami` 查看当前用户。

## 限制

本技能不涵盖：
- 组织管理功能
- 管理员操作（需要 admin 命令）
- Webhook 管理
- CI/CD 配置
- 时间跟踪（tea times 功能）

这些功能如需要，请参考 Gitea 官方文档或 Web 界面。

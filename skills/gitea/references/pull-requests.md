# Gitea Pull Request 管理

## 列出 Pull Request

### 列出当前仓库的 PR
```bash
tea pr list
```

**选项：**
- `--state`：按状态过滤（open, closed, all, merged）
- `--page`：页码
- `--limit`：每页数量限制

**示例：**
```bash
# 列出所有开放的 PR
tea pr list --state open

# 列出已合并的 PR
tea pr list --state merged

# 列出所有 PR（包括已关闭）
tea pr list --state all
```

### 在其他仓库中列出 PR
```bash
tea pr list --repo owner/repo
```

## 查看 Pull Request

### 查看 PR 详情
```bash
tea pr <PR编号>
```

**示例：**
```bash
# 查看 PR #123
tea pr 123
```

## 创建 Pull Request

### 从当前分支创建 PR
```bash
tea pr create --title "PR 标题" --description "PR 描述"
```

**选项：**
- `--title`：PR 标题（必需）
- `--description` 或 `--desc`：PR 描述
- `--head`：源分支（默认为当前分支）
- `--base`：目标分支（默认为仓库默认分支，通常是 main）

**示例：**
```bash
# 从当前分支创建 PR 到 main
tea pr create --title "Add new feature" --description "This PR adds..."

# 创建到特定目标分支的 PR
tea pr create --title "Fix bug" --base develop

# 从特定源分支创建 PR
tea pr create --title "Update docs" --head feature/docs-update --base main
```

### 使用文件创建 PR
```bash
tea pr create --title "标题" --description "$(cat pr-description.md)"
```

## Pull Request Checkout

### Checkout PR 到本地
```bash
tea pr checkout <PR编号>
```

这会创建并切换到对应的本地分支，便于代码审查和测试。

**示例：**
```bash
# Checkout PR #42
tea pr checkout 42

# 这会创建类似 pr-42 的本地分支并切换到它
```

## 编辑 Pull Request

### 编辑 PR
```bash
tea pr edit <PR编号> --title "新标题" --description "新描述"
```

**选项：**
- `--title`：新标题
- `--description` 或 `--desc`：新描述内容
- `--state`：更改状态（open, closed）

**示例：**
```bash
# 更改标题
tea pr edit 42 --title "Updated PR title"

# 关闭 PR
tea pr edit 42 --state closed

# 重新打开 PR
tea pr edit 42 --state open

# 更新描述
tea pr edit 42 --description "$(cat updated-description.md)"
```

## 合并 Pull Request

### 合并 PR
```bash
tea pr merge <PR编号>
```

**选项：**
- `--merge`：使用合并提交（默认）
- `--rebase`：使用 rebase 方式合并
- `--squash`：使用 squash 方式合并
- `--delete-branch`：合并后删除分支

**示例：**
```bash
# 使用合并提交合并
tea pr merge 42 --merge

# 使用 rebase 合并
tea pr merge 42 --rebase

# 使用 squash 合并并删除分支
tea pr merge 42 --squash --delete-branch

# 直接使用默认方式合并
tea pr merge 42
```

## 评论 Pull Request

### 添加 PR 评论
```bash
tea comment <PR编号> --body "评论内容"
```

**示例：**
```bash
# 添加简单评论
tea comment 42 --body "LGTM! Looks good to me."

# 从文件添加评论
tea comment 42 --body "$(cat review-comment.md)"
```

## Pull Request 工作流

### 标准工作流
```bash
# 1. 创建功能分支
git checkout -b feature/new-feature

# 2. 进行更改并提交
git add .
git commit -m "Add new feature"

# 3. 推送到远程
git push -u origin feature/new-feature

# 4. 创建 PR
tea pr create --title "Add new feature" --description "Description here"

# 5. 等待审查和合并
```

### 审查 PR
```bash
# 1. Checkout PR 到本地进行测试
tea pr checkout 42

# 2. 测试和审查代码

# 3. 添加评论
tea comment 42 --body "Review comments..."

# 4. 如果需要，更新 PR 源分支
git checkout feature/new-feature
git commit --amend
git push force

# 5. 合并 PR
tea pr merge 42 --squash
```

## 最佳实践

1. **分支命名**：使用描述性的分支名（如 `feature/xxx`, `bugfix/xxx`）
2. **PR 标题**：使用祈使句，清晰描述更改（如 "Add login feature" 而非 "Added login feature"）
3. **PR 描述**：包含更改摘要、相关 Issue 链接、测试说明
4. **小步提交**：保持 PR 聚焦和规模适中
5. **及时审查**：创建 PR 后及时请求审查

## 常用命令组合

```bash
# 创建并审查 PR
git push -u origin feature/new-feature && tea pr create --title "..." --description "..."

# 快速合并已审查的 PR
tea pr checkout 42
# 测试...
tea pr merge 42 --squash --delete-branch

# 查看并关闭不需要的 PR
tea pr list --state open
tea pr edit 42 --state closed
```

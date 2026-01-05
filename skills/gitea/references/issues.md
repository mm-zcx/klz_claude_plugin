# Gitea Issue 管理

## 列出 Issue

### 列出当前仓库的 Issue
```bash
tea issue list
```

**选项：**
- `--state`：按状态过滤（open, closed, all）
- `--page`：页码
- `--limit`：每页数量限制
- `--labels`：按标签过滤
- `--milestone`：按里程碑过滤

**示例：**
```bash
# 列出所有开放的 Issue
tea issue list --state open

# 列出已关闭的 Issue
tea issue list --state closed

# 列出特定标签的 Issue
tea issue list --labels bug,high-priority

# 列出特定里程碑的 Issue
tea issue list --milestone "v1.0.0"
```

### 在其他仓库中列出 Issue
```bash
tea issue list --repo owner/repo
```

## 查看 Issue

### 查看 Issue 详情
```bash
tea issue <issue编号>
```

**示例：**
```bash
# 查看 Issue #42
tea issue 42
```

## 创建 Issue

### 创建新 Issue
```bash
tea issue create --title "标题" --body "描述内容"
```

**选项：**
- `--title`：Issue 标题（必需）
- `--body` 或 `--desc`：Issue 描述内容
- `--labels`：添加标签（逗号分隔）
- `--milestone`：关联里程碑

**示例：**
```bash
# 创建简单 Issue
tea issue create --title "Bug in login" --body "Login fails on Chrome"

# 创建带标签的 Issue
tea issue create --title "Fix navigation" --body "Nav menu broken" --labels bug,high-priority

# 创建关联里程碑的 Issue
tea issue create --title "Add feature" --milestone "v1.0.0"
```

### 从文件创建 Issue
```bash
tea issue create --title "标题" --body "$(cat issue-template.md)"
```

## 编辑 Issue

### 编辑 Issue
```bash
tea issue edit <issue编号> --title "新标题" --body "新描述"
```

**选项：**
- `--title`：新标题
- `--body` 或 `--desc`：新描述内容
- `--state`：更改状态（open, closed）
- `--labels`：更新标签（逗号分隔）
- `--milestone`：更新里程碑

**示例：**
```bash
# 更改标题
tea issue edit 42 --title "Updated title"

# 关闭 Issue
tea issue edit 42 --state closed

# 重新打开 Issue
tea issue edit 42 --state open

# 更新标签
tea issue edit 42 --labels bug,confirmed

# 更新描述
tea issue edit 42 --body "$(cat updated-description.md)"
```

## 评论 Issue

### 添加评论
```bash
tea comment <issue编号> --body "评论内容"
```

**示例：**
```bash
# 添加简单评论
tea comment 42 --body "I'm working on this"

# 从文件添加评论
tea comment 42 --body "$(cat comment.txt)"
```

## 批量操作

### 批量关闭 Issue
```bash
tea issue list --state open --labels bug | tea issue edit --state closed
```

### 按里程碑列出 Issue
```bash
tea issue list --milestone "v1.0.0" --state all
```

## Issue 工作流建议

1. **使用标签**：为 Issue 分配合适的标签（bug, enhancement, question 等）
2. **关联里程碑**：将 Issue 与发布里程碑关联以便跟踪进度
3. **描述模板**：使用一致的 Issue 描述格式
4. **及时更新**：Issue 状态变更时及时更新

## 常用标签组合

```bash
# Bug 报告
tea issue create --title "Bug: ..." --labels bug,needs-confirmation

# 功能请求
tea issue create --title "Feature: ..." --labels enhancement,good-first-issue

# 高优先级
tea issue create --title "Critical: ..." --labels bug,high-priority
```

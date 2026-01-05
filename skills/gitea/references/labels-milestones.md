# Gitea 标签和里程碑管理

## 标签（Labels）管理

### 列出标签
```bash
tea labels list
```

列出当前仓库的所有标签。

### 在其他仓库中列出标签
```bash
tea labels list --repo owner/repo
```

### 创建标签
```bash
tea labels create --name "<标签名>" --description "<描述>" --color <颜色>
```

**选项：**
- `--name`：标签名称（必需）
- `--description` 或 `--desc`：标签描述
- `--color`：标签颜色（十六进制颜色代码，如 #0052CC）
- `--repo`：指定仓库（默认为当前仓库）

**示例：**
```bash
# 创建基本标签
tea labels create --name bug --description "Bug report" --color "#d73a4a"

# 创建功能请求标签
tea labels create --name enhancement --description "New feature or enhancement" --color "#a2eeef"

# 创建高优先级标签
tea labels create --name "high-priority" --description "High priority issue" --color "#ff9933"

# 创建文档标签
tea labels create --name documentation --color "#0075ca"
```

### 更新标签
```bash
tea labels update <旧标签名> --name "<新标签名>" --color <新颜色>
```

**示例：**
```bash
# 更新标签颜色
tea labels update bug --color "#ff0000"

# 重命名标签
tea labels update bug --name "bug-report"

# 更新标签描述和颜色
tea labels update enhancement --description "Feature request" --color "#00ff00"
```

### 删除标签
```bash
tea labels delete <标签名>
```

**示例：**
```bash
# 删除标签
tea labels delete old-label
```

## 常用标签建议

### 标准标签集
```bash
# 类型标签
tea labels create --name bug --description "Bug report" --color "#d73a4a"
tea labels create --name enhancement --description "New feature or enhancement" --color "#a2eeef"
tea labels create --name question --description "Question or help request" --color "#d876e3"
tea labels create --name documentation --description "Documentation improvement" --color "#0075ca"

# 优先级标签
tea labels create --name "high-priority" --description "High priority" --color "#ff9933"
tea labels create --name "low-priority" --description "Low priority" --color "#e6e6e6"

# 状态标签
tea labels create --name "good-first-issue" --description "Good for newcomers" --color "#7057ff"
tea labels create --name "wontfix" --description "Won't fix" --color "#ffffff"
tea labels create --name duplicate --description "Duplicate issue" --color "#cfd3d7"

# 工作流标签
tea labels create --name "in-progress" --description "Currently being worked on" --color "#fbca04"
tea labels create --name "needs-review" --description "Needs review" --color "#5319e7"
tea labels create --name "approved" --description "Approved" --color "#0e8a16"
```

## 里程碑（Milestones）管理

### 列出里程碑
```bash
tea milestones list
```

列出当前仓库的所有里程碑。

### 在其他仓库中列出里程碑
```bash
tea milestones list --repo owner/repo
```

### 创建里程碑
```bash
tea milestones create --title "<里程碑标题>" --description "<描述>"
```

**选项：**
- `--title`：里程碑标题（必需）
- `--description` 或 `--desc`：里程碑描述
- `--deadline`：截止日期（格式：YYYY-MM-DD）
- `--state`：状态（open, closed, all）
- `--repo`：指定仓库（默认为当前仓库）

**示例：**
```bash
# 创建基本里程碑
tea milestones create --title "v1.0.0 Release" --description "First major release"

# 创建带截止日期的里程碑
tea milestones create --title "v1.1.0" --deadline "2026-03-31"

# 创建迭代里程碑
tea milestones create --title "Sprint 42" --description "Two-week sprint" --deadline "2026-01-17"
```

### 更新里程碑
```bash
tea milestones update <里程碑标题> --title "<新标题>" --state <状态>
```

**示例：**
```bash
# 更新标题
tea milestones update "v1.0.0 Release" --title "v1.0.0"

# 更新截止日期
tea milestones update "v1.1.0" --deadline "2026-04-30"

# 关闭里程碑
tea milestones update "v1.0.0" --state closed

# 重新打开里程碑
tea milestones update "v1.0.0" --state open

# 更新描述
tea milestones update "Sprint 42" --description "Extended sprint"
```

### 删除里程碑
```bash
tea milestones delete <里程碑标题>
```

**示例：**
```bash
# 删除里程碑
tea milestones delete "Cancelled Sprint"
```

## 使用标签和里程碑

### 在创建 Issue 时使用标签和里程碑
```bash
# 创建带标签和里程碑的 Issue
tea issue create --title "Fix bug" \
  --labels bug,high-priority \
  --milestone "v1.0.0" \
  --body "Bug description here"
```

### 在创建 PR 时使用标签和里程碑
```bash
# 创建带里程碑的 PR（tea 不直接支持 PR 标签，可通过 Web 界面添加）
tea pr create --title "Feature" \
  --milestone "v1.1.0" \
  --description "PR description"
```

### 筛选带有特定标签或里程碑的 Issue
```bash
# 按标签筛选
tea issue list --labels bug

# 按多个标签筛选
tea issue list --labels bug,high-priority

# 按里程碑筛选
tea issue list --milestone "v1.0.0"

# 组合筛选
tea issue list --labels enhancement --milestone "v1.1.0" --state open
```

## 标签和里程碑工作流

### 里程碑工作流
```bash
# 1. 为版本创建里程碑
tea milestones create --title "v1.0.0" --deadline "2026-03-31"

# 2. 为相关 Issue 分配里程碑
tea issue create --title "Feature A" --milestone "v1.0.0"
tea issue create --title "Bug B" --labels bug --milestone "v1.0.0"

# 3. 跟踪里程碑进度
tea issue list --milestone "v1.0.0" --state all

# 4. 完成后关闭里程碑
tea milestones update "v1.0.0" --state closed

# 5. 创建发布版本
tea release create --tag v1.0.0 --title "Version 1.0.0"
```

### 标签工作流
```bash
# 1. 设置初始标签集
tea labels create --name bug --color "#d73a4a"
tea labels create --name enhancement --color "#a2eeef"

# 2. 创建 Issue 时分配标签
tea issue create --title "New feature" --labels enhancement

# 3. 更新 Issue 状态标签
tea issue edit 42 --labels "in-progress"

# 4. 完成后更新标签
tea issue edit 42 --labels "approved,merged"
```

## 最佳实践

### 标签管理
1. **标准化标签集**：为项目建立一致的标签系统
2. **颜色编码**：使用颜色区分不同类型（红色=bug，绿色=功能等）
3. **限制数量**：避免创建过多标签，保持简洁
4. **定期清理**：删除未使用的标签

### 里程碑管理
1. **清晰命名**：使用版本号或迭代号命名（v1.0.0, Sprint 42）
2. **设置截止日期**：为里程碑设置合理的截止日期
3. **合理范围**：每个里程碑包含适当数量的 Issue（通常 5-20 个）
4. **及时关闭**：完成后及时关闭里程碑

### 组合使用
1. **优先级标签**：结合优先级标签和里程碑进行排序
2. **状态跟踪**：使用状态标签（in-progress, needs-review）跟踪进度
3. **版本规划**：使用里程碑规划版本，使用标签分类 Issue

## 常用命令组合

```bash
# 查看特定里程碑的所有 Issue
tea issue list --milestone "v1.0.0" --state all

# 查看高优先级 bug
tea issue list --labels bug,high-priority --state open

# 为当前 Sprint 创建里程碑并分配 Issue
tea milestones create --title "Sprint 43" --deadline "2026-01-31"
tea issue edit 42 --milestone "Sprint 43"

# 完成里程碑后创建发布版本
tea milestones update "v1.0.0" --state closed
git tag -a v1.0.0 -m "Release v1.0.0"
git push origin v1.0.0
tea release create --tag v1.0.0 --title "Version 1.0.0"
```

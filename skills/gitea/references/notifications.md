# Gitea 通知和辅助功能

## 通知管理

### 列出通知
```bash
tea notification list
```

列出当前用户的所有通知。

**选项：**
- `--states`：按状态过滤（pinned, unread, read）
- `--page`：页码
- `--limit`：每页数量限制

**示例：**
```bash
# 列出所有通知
tea notification list

# 列出未读通知
tea notification list --states unread

# 列出已置顶的通知
tea notification list --states pinned

# 列出已读和未读通知
tea notification list --states read,unread
```

### 读取通知
```bash
tea notification read <通知编号>
```

标记通知为已读。

**示例：**
```bash
# 标记特定通知为已读
tea notification read 1

# 标记所有通知为已读
tea notification read --all
```

### 置顶通知
```bash
tea notification pin <通知编号>
```

将重要通知置顶。

**示例：**
```bash
# 置顶通知
tea notification pin 1
```

### 取消置顶通知
```bash
tea notification unpin <通知编号>
```

**示例：**
```bash
# 取消置顶
tea notification unpin 1
```

### 删除通知
```bash
tea notification delete <通知编号>
```

**示例：**
```bash
# 删除特定通知
tea notification delete 1

# 删除所有通知
tea notification delete --all
```

## 在浏览器中打开资源

### 打开仓库主页
```bash
tea open
```

在默认浏览器中打开当前仓库的主页。

### 打开特定页面
```bash
# 打开 Issues 列表
tea open issues

# 打开 Pull Requests 列表
tea open pulls

# 打开 Releases 列表
tea open releases

# 打开 Milestones 列表
tea open milestones

# 打开 Packages 列表
tea open packages

# 打开 Wiki
tea open wiki

# 打开 Actions/CI
tea open actions

# 打开仓库设置
tea open settings
```

### 打开特定分支
```bash
# 打开 main 分支
tea open src/branch/main

# 打开 develop 分支
tea open src/branch/develop
```

### 打开特定文件
```bash
# 打开文件
tea open src/branch/main/README.md

# 打开目录
tea open src/branch/main/src
```

### 打开特定 Issue 或 PR
```bash
# 打开 Issue
tea open issues/42

# 打开 Pull Request
tea open pulls/123
```

### 打开特定提交
```bash
# 打开提交详情
tea open commits/abc123def456
```

### 打开特定标签
```bash
# 打开标签
tea open src/tag/v1.0.0

# 打开标签的发布版本
tea open releases/tag/v1.0.0
```

### 打开用户或组织
```bash
# 打开当前用户
tea open https://gitea.com/username

# 打开特定组织
tea open https://gitea.com/org-name
```

## 用户身份

### 查看当前登录用户
```bash
tea whoami
```

显示当前登录的用户信息，包括用户名、邮箱等。

**示例：**
```bash
$ tea whoami
mm-kzl <mm-kzl@example.com>
```

## 登录管理

### 登录到 Gitea 服务器
```bash
tea login add
```

交互式登录流程：
1. 输入 Gitea 服务器 URL（如 https://gitea.com）
2. 输入用户名或邮箱
3. 输入密码或使用 token
4. 为登录命名（可选）

**示例：**
```bash
# 添加默认登录
tea login add

# 添加命名的登录
tea login add --name work

# 指定服务器 URL
tea login add --url https://gitea.com

# 使用 token 登录（推荐）
tea login add --token <your-token>
```

### 列出所有登录
```bash
tea logins list
```

显示所有已保存的 Gitea 登录。

### 删除登录
```bash
tea login delete <登录名称>
```

**示例：**
```bash
# 删除特定登录
tea login delete work
```

### 注销
```bash
tea logout
```

从当前 Gitea 服务器注销。

## 多实例管理

### 为特定操作指定登录
```bash
# 为特定仓库使用特定登录
tea repo --login work

# 为 Issue 操作使用特定登录
tea issue list --login personal
```

### 查看当前配置
```bash
# 查看配置文件位置
tea config

# 通常位于：
# macOS: ~/Library/Application Support/tea/config.yml
# Linux: ~/.config/tea/config.yml
# Windows: %APPDATA%/tea/config.yml
```

## 辅助功能工作流

### 通知管理工作流
```bash
# 1. 查看所有未读通知
tea notification list --states unread

# 2. 置顶重要通知
tea notification pin 5

# 3. 标记为已读
tea notification read 1

# 4. 清理已处理的通知
tea notification delete --all
```

### 快速导航工作流
```bash
# 1. 查看当前仓库状态
tea repo

# 2. 在浏览器中打开仓库
tea open

# 3. 打开 Issues 进行处理
tea open issues

# 4. 创建新 Issue
tea issue create --title "..." --body "..."

# 5. 查看未读通知
tea notification list --states unread
```

### 多账户工作流
```bash
# 1. 添加多个登录
tea login add --name work --url https://work-gitea.com
tea login add --name personal --url https://gitea.com

# 2. 使用特定登录
tea issue list --login work

# 3. 查看当前用户
tea whoami --login work

# 4. 切换登录
tea login set-default work
```

## 最佳实践

### 通知管理
1. **定期清理**：定期清理已读通知，保持列表简洁
2. **置顶重要**：置顶需要后续跟进的重要通知
3. **及时标记**：处理通知后及时标记为已读

### 浏览器集成
1. **快速访问**：使用 `tea open` 快速在浏览器中打开资源
2. **代码审查**：使用 `tea pr checkout` + `tea open` 结合进行代码审查
3. **问题跟踪**：使用 `tea open issues` 查看和管理 Issue

### 多实例
1. **命名清晰**：为不同登录使用清晰的名称（work, personal, client）
2. **分离关注点**：工作和个人项目使用不同的 Gitea 实例
3. **权限管理**：使用 token 而非密码进行认证

## 常用命令组合

```bash
# 查看通知并打开相关 Issue
tea notification list --states unread
tea open issues/42

# 查看仓库并在浏览器中打开
tea repo
tea open

# 快速创建 Issue 并查看
tea issue create --title "Bug" --body "Description"
tea open issues

# 管理多个账户
tea whoami
tea logins list
tea issue list --login personal
```

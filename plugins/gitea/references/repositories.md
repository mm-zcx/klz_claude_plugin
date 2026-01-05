# Gitea 仓库管理

## 查看仓库信息

### 查看当前仓库详情
```bash
tea repo
```

显示当前仓库的基本信息，包括名称、描述、所有者等。

### 查看仓库设置
```bash
tea repo view --repo owner/repo
```

## 创建仓库

### 创建新仓库
```bash
tea repo create --name <仓库名> --description "<描述>" --owner <所有者>
```

**选项：**
- `--name`：仓库名称（必需）
- `--description` 或 `--desc`：仓库描述
- `--owner` 或 `-O`：仓库所有者（默认为当前用户）
- `--private`：创建私有仓库（默认为公开）
- `--init`：初始化仓库（添加 README、.gitignore 等）
- `--branch`：指定默认分支名（需要 --init）
- `--license`：添加许可证（需要 --init）
- `--gitignores` 或 `--git`：使用 .gitignore 模板（需要 --init）
- `--readme`：使用 README 模板（需要 --init）
- `--template`：创建为模板仓库

**示例：**
```bash
# 创建公开仓库
tea repo create --name my-project --description "My new project"

# 创建私有仓库
tea repo create --name private-project --description "Private repo" --private

# 创建并初始化的仓库
tea repo create --name initialized-project --init --readme "Default" --license MIT
```

## 克隆仓库

### 克隆仓库到本地
```bash
tea clone <仓库地址>
```

**示例：**
```bash
# 克隆仓库
tea clone https://gitea.com/owner/repo.git

# 克隆到指定目录
tea clone https://gitea.com/owner/repo.git my-directory
```

## 分支管理

### 列出分支
```bash
tea branch
```

列出当前仓库的所有分支。

### 查看特定分支
```bash
tea branch --branch <分支名>
```

**选项：**
- `--branch` 或 `-b`：分支名称
- `--repo`：指定仓库（默认为当前仓库）

## 在浏览器中打开

### 打开仓库页面
```bash
tea open
```

在默认浏览器中打开当前仓库的页面。

### 打开特定页面
```bash
# 打开 Issues 页面
tea open issues

# 打开 Pull Requests 页面
tea open pulls

# 打开 Releases 页面
tea open releases

# 打开特定分支
tea open src/branch/main

# 打开特定文件
tea open src/branch/main/README.md
```

## 仓库配置

### 配置远程仓库
tea 会自动检测当前目录的 git 配置。确保本地 git 仓库已配置正确的远程：

```bash
git remote add origin <gitea-repo-url>
git push -u origin main
```

## 工作流提示

1. **最佳实践**：tea 假设本地 git 状态已在远程发布，因此在执行 tea 操作前先提交并推送本地更改
2. **上游/分支工作流**：tea 在本地 main 分支跟踪上游仓库时工作最佳
3. **上下文感知**：tea 会使用 $PWD 中提供的仓库上下文（如果可用）

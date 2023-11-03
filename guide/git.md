# 如何使用 Git

## 1. 安装 Git

下载并安装 [Git](https://git-scm.com/downloads)。

在 VSCode 中安装 [GitLens](https://marketplace.visualstudio.com/items?itemName=eamodio.gitlens) 插件和 [Conventional Commits](https://marketplace.visualstudio.com/items?itemName=vivaxy.vscode-conventional-commits) 插件。

## 2. 配置 Git

配置 Git 的用户名和邮箱：

```bash
git config --global user.name "你的用户名"
git config --global user.email "你的邮箱"
```

配置 SSH：

```bash
ssh-keygen -t rsa -C "你的邮箱"
```

将 `C:\Users\你的用户名\.ssh\id_rsa.pub` 中的内容复制到 GitHub 的 SSH keys 中。

## 3. 使用 Git（命令行）

### 3.1. 克隆仓库

```bash
git clone 你的仓库地址
```

### 3.2. 添加文件

```bash
git add 你的文件
```

### 3.3. 提交文件

```bash
git commit -m "你的提交信息"
```

### 3.4. 推送文件

```bash
git push
```

### 3.5. 拉取文件

```bash
git pull
```

### 3.6. 查看状态

```bash
git status
```

## 4. 使用 Git（VSCode）

用 VSCode 打开仓库，点击左边的“源代码管理”，点击“约定式提交”，输入提交信息，提交。

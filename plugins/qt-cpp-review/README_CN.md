# Qt C++ 代码审查技能

## 技能概述

这是一个用于审查Qt C++代码质量的Claude技能,基于Qt官方编码规范、C++最佳实践以及项目自定义规范。

## 功能特性

### 审查维度

1. **Qt特定规范**
   - 信号槽连接方式
   - 对象树和内存管理
   - Qt容器使用
   - 跨线程通信
   - 国际化支持

2. **C++最佳实践**
   - 内存管理
   - RAII原则
   - const正确性
   - 异常安全

3. **性能优化**
   - 避免不必要的拷贝
   - 隐式共享利用
   - 字符串处理优化
   - UI性能优化

4. **代码风格**
   - 命名规范
   - 代码格式
   - 注释质量
   - 头文件组织

## 技能结构

```
qt-cpp-review/
├── SKILL.md                          # 技能主文件
├── qt-cpp-review.skill               # 打包的技能文件
├── references/                       # 参考文档
│   ├── qt-coding-standards.md       # Qt官方编码规范
│   ├── project-standards.md         # 项目自定义规范
│   └── checklist.md                 # 详细检查清单
├── scripts/                          # 脚本目录(当前为空)
└── assets/                           # 资源目录(当前为空)
```

## 使用方法

### 安装技能

1. 将`qt-cpp-review.skill`文件复制到Claude技能目录
2. 在Claude Code中加载该技能

### 审查单个文件

```
请审查以下Qt C++文件的代码质量
[粘贴代码或提供文件路径]
```

### 审查项目整体

```
请审查以下Qt项目的代码规范遵循情况
[提供项目路径或描述项目结构]
```

### 特定维度审查

```
请检查以下代码的内存管理问题
[粘贴代码]

请分析以下Qt代码的性能优化空间
[粘贴代码]

请审查信号槽连接是否正确
[粘贴代码]
```

### 编码规范检查

```
请检查以下代码是否符合项目编码规范
[粘贴代码]
重点关注: 命名规范、大括号使用、注释完整性
```

## 项目编码规范要点

### 命名规范

- **类名**: C前缀 + 大驼峰 (例: `CMyClass`)
- **成员变量**: m_前缀 + 小驼峰 (例: `m_myValue`)
- **全局变量**: g_前缀 + 小驼峰 (例: `g_myValue`)
- **常量**: 全大写下划线 (例: `MAX_COUNT`)
- **函数**: 小驼峰 (例: `getMyValue()`)
- **函数参数**: 小写下划线 (例: `new_color`)

### 代码格式

- **缩进**: 4空格(不用Tab)
- **大括号**: 独占一行
- **行长度**: < 100字符
- **空行**: 独立代码块间必须加空行

### Qt最佳实践

- 使用函数指针连接信号槽(避免SIGNAL/SLOT宏)
- 利用对象树管理内存(指定父对象)
- const引用传递大对象
- GUI操作仅限主线程
- 使用tr()支持国际化

## 审查输出

审查结果包含:

1. **总体评估**: 代码质量概况
2. **问题清单**: 按优先级分类
   - 🔴 严重问题(Must Fix)
   - 🟡 重要问题(Should Fix)
   - 🟢 建议改进(Nice to Have)
3. **改进建议**: 具体代码修改示例
4. **规范引用**: 相关编码规范链接

## 常见问题示例

### ❌ 错误示例

```cpp
// 类名缺少C前缀
class MySystem {
    int value;  // 成员变量缺少m_前缀
};

// 使用旧式信号槽连接
connect(sender, SIGNAL(valueChanged(QString)),
        receiver, SLOT(updateValue(QString)));

// 未指定父对象
QPushButton *button = new QPushButton();

// 大括号不独占一行
if (condition) {
    return;
}
```

### ✅ 正确示例

```cpp
// 类名有C前缀
class CMySystem {
    int m_value;  // 成员变量有m_前缀
};

// 使用函数指针连接
connect(sender, &Sender::valueChanged,
        receiver, &Receiver::updateValue);

// 指定父对象
QPushButton *button = new QPushButton(parent);

// 大括号独占一行
if (condition)
{
    return;
}
```

## 版本信息

- **版本**: 0.1.0
- **支持Qt版本**: Qt 5 & Qt 6
- **创建日期**: 2025-01-01
- **维护者**: mm-kzl

## 更新日志

### v0.1.0 (2025-01-01)
- 初始版本发布
- 支持Qt 5和Qt 6
- 包含Qt官方编码规范
- 包含项目自定义编码规范
- 详细的代码审查检查清单

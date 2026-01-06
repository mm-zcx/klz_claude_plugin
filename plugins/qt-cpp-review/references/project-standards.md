# 项目编码规范(自定义)

## 概述

本规范参考Effective C++中文版、Google C++编码规范及Qt编码风格。

**重要性优先级**: 正确性 > 稳定性 > 可测试性 > 可读性 > 全局效率 > 局部效率 > 个人习惯

## 1. 头文件规范

### 1.1 #define保护
- 所有头文件使用#define防止多重包含
- 命名格式: `<ProjectName>_<FileName>_H`
```cpp
#ifndef MAP_MAINWINDOW_H
#define MAP_MAINWINDOW_H
// ...
#endif // MAP_MAINWINDOW_H
```

### 1.2 头文件依赖
- 使用前向声明减少.h文件中#include数量
- 在.cpp中包含完整定义

### 1.3 头文件包含次序
1. C库头文件
2. C++库头文件
3. 其他库的头文件
4. 项目内的头文件

```cpp
// 示例
#include <stdio.h>           // C库
#include <QString>           // C++库
#include <thirdparty/lib.h>  // 其他库
#include "myclass.h"         // 项目内
```

## 2. 命名约定

### 2.1 通用原则
- 避免使用缩写
- 使用描述性名称

### 2.2 文件命名
- 文件名全部小写
- 可以包含下划线
```cpp
// 可接受的命名
my_test_class.h/cpp
mytestclass.h/cpp
```

### 2.3 类命名
- 类名是名词,每个单词以大写字母开头
- 不包含下划线
- **类名前加大写字母C**
```cpp
// ✅ 正确
class CMySystemWatcher {}
class CClockBattery {}

// ❌ 错误
class MySystemWatcher {}
class my_system_watcher {}
```

### 2.4 变量命名
- 变量名是名词
- 首单词小写,后续单词大写开头(小驼峰)
- 每行一个变量
- 单字符变量仅用于临时变量或循环计数

**成员变量**:
- 必须加`m_`前缀
```cpp
// ✅ 正确
int m_myValue;
QString m_userName;

// ❌ 错误
int myValue;        // 缺少m_前缀
int m_my_value;     // 应使用驼峰
```

**局部变量**:
- 等到需要时再定义
- 定义时必须初始化
```cpp
// ✅ 正确
int myValue = 0;
QString name = QString(); // 或 ""
QString *ptr = NULL;

// ❌ 错误
int myValue;
QString name;
```

**全局变量**:
- 尽量避免使用
- 若使用,加`g_`前缀
```cpp
int g_myValue;
```

### 2.5 常量命名
- 常量不含前缀
- 全部大写
- 单词间用下划线分隔
```cpp
// ✅ 正确
const int MY_VALUE = 0;
#define MAX_COUNT 100

// ❌ 错误
const int myValue = 0;
const int MyValue = 0;
```

### 2.6 函数命名
- 函数名是动词或含动词的短语
- 首单词小写,后续单词大写开头
```cpp
// ✅ 正确
int getMyValue();
int setMyValue(int value);

// ❌ 错误
int GetMyValue();     // 不应首字母大写
int get_my_value();   // 不应使用下划线
```

**函数参数**:
- 所有字母小写
- 单词间有下划线
- 非基本类型使用const引用
```cpp
// ✅ 正确
void setColor(const QColor &new_color);
void setPosition(int x, int y);

// ❌ 错误
void setColor(const QColor &newColor);  // 参数应为小写下划线
void setColor(QColor newColor);         // 应使用const引用
```

### 2.7 枚举命名
- 枚举名: 名词,每个单词大写开头,第一个单词是Enum
- 枚举值: 名词,每个单词大写开头,第一个单词是Enum
```cpp
// ✅ 正确
enum EnumMyColor {
    EnumWhite,
    EnumBlack
};

// ❌ 错误
enum MyColor {      // 缺少Enum前缀
    White,          // 缺少Enum前缀
    BLACK
};
```

### 2.8 命名空间
- 名词,每个单词大写开头
- 前两个单词是BaiDi(或项目名)
```cpp
namespace BaiDiProject {}
```

### 2.9 结构体命名
- 结构体只定义变量,不定义函数
- 结构体名: 名词,每个单词大写开头
- 成员名: 首单词小写,后续单词大写开头
```cpp
// ✅ 正确
struct MyColor {
    bool isMyColor;
    int white;
};

// ❌ 错误
struct MyColor {
    bool m_isMyColor;  // 结构体不应使用m_前缀
    void setColor() {} // 结构体不应定义函数
};
```

## 3. 代码注释

### 3.1 注释风格
- 使用`//`或`/**/`,统一即可
- 源程序有效注释量必须在20%以上
- 不易理解的地方必须加注释

### 3.2 类注释
头文件顶部必须包含:
```cpp
/*****************************************************
**Copyright(C), 2015-2025, Baodi Technology.
**Version: 1.0
**Author: 张三
**Date: 2025-01-01
**Description: 系统监控类,负责监控系统状态
*****************************************************/
```

### 3.3 函数注释
重要函数头部必须包含:
```cpp
/*************************************************
**Function: getMyValue
**Description: 获取当前系统值
**Input: value - 需要设置的值
**Output: 无
**Return: 成功返回true,失败返回false
**Others: 需在调用前初始化系统
*************************************************/
bool setMyValue(int value)
{
    // 实现
}
```

### 3.4 变量注释
- 通常变量名足以说明用途
- 特定情况需要额外注释
- 注释位于变量上方
```cpp
// keeps track of the total number of entries in the table
int totalNumber;
```

### 3.5 实现注释
- 巧妙、晦涩、重要的代码需注释
- 复杂代码块上方注释
```cpp
// Divide result by two, taking into account that x
// contains the carry from the add.
for (int i = 0; i < 100; i++) {
    x = (x << 8) + (*result)[i];
    (*result) = x >> 1;
    x &= 1;
}
```
- 晦涩地方可在行尾注释(代码后空两格)
```cpp
if (condition) {
    return;  // return while meeting the conditions
}
```

### 3.6 TODO注释
```cpp
void CMyClass::function() {
    // TODO: 实现待办事项
}
```

## 4. 代码排版

### 4.1 行长度
- 较长语句(>80字符)分成多行
- 在低优先级操作符处划分
- 操作符放新行首,逗号放行尾
- 适当缩进
```cpp
// ✅ 正确
if ((taskOne < taskNumber) && (taskTwo < taskNumber)
    && (taskThree < taskNumber)) {
    // ...
}
```

### 4.2 缩进
- 使用4个空格(不用Tab)
- 预处理指令不缩进,顶格开始
```cpp
if (isActive()) {
#if DISASTER_PENDING
    dropEverything();
#endif
    backToNormal();
}
```

### 4.3 空行
- 适当使用空行分组
- 相对独立代码块间必须加空行

### 4.4 大括号
- **大括号永远单独占一行**
- 不跟在语句后面
- 控制语句体为空或只有一行,也要使用大括号
```cpp
// ✅ 正确
if (address.isEmpty()) {
    return false;
}

switch (value) {
case 0:
    {
        getElectricity();
        break;
    }
default:
    {
        break;
    }
}

// ❌ 错误
if (address.isEmpty()) {
    return false;  // 缺少大括号
}
```

### 4.5 圆括号
- 使用圆括号分组表达式
- 即使优先级相同也要分组
```cpp
// ✅ 正确
if ((a && b) || c)

// ❌ 错误
if (a && b || c)
```

## 5. 常见错误示例

### ❌ 错误示例1: 类名缺少C前缀
```cpp
// 错误
class MySystem {}

// 正确
class CMySystem {}
```

### ❌ 错误示例2: 成员变量缺少m_前缀
```cpp
class CMyClass {
    // 错误
    int value;
    QString name;

    // 正确
    int m_value;
    QString m_name;
};
```

### ❌ 错误示例3: 大括号不独占一行
```cpp
// 错误
if (condition) {
    return;
}

// 正确
if (condition)
{
    return;
}
```

### ❌ 错误示例4: 函数参数命名不规范
```cpp
// 错误
void setColor(const QColor &newColor);

// 正确
void setColor(const QColor &new_color);
```

### ❌ 错误示例5: 局部变量未初始化
```cpp
// 错误
int value;
QString name;

// 正确
int value = 0;
QString name = QString();
```

## 6. 审查重点

1. **类名**: 必须有C前缀
2. **成员变量**: 必须有m_前缀
3. **全局变量**: 必须有g_前缀
4. **常量**: 必须全大写
5. **函数参数**: 必须小写下划线命名
6. **大括号**: 必须独占一行
7. **注释**: 类和重要函数必须有完整注释
8. **初始化**: 局部变量必须初始化

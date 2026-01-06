# Qt C++ 代码审查检查清单

## 使用说明

本文档提供了详细的代码审查检查项。按需加载相应的检查清单部分。

## 目录

- [命名规范检查](#命名规范检查)
- [内存管理检查](#内存管理检查)
- [信号槽检查](#信号槽检查)
- [线程安全检查](#线程安全检查)
- [性能优化检查](#性能优化检查)
- [代码风格检查](#代码风格检查)
- [架构设计检查](#架构设计检查)
- [Qt特定规范检查](#qt特定规范检查)

---

## 命名规范检查

### ✅ 类命名
- [ ] 类名使用大驼峰命名法(CamelCase)
- [ ] 类名是名词而非动词
- [ ] 抽象类命名清晰或使用Interface/Base后缀

**示例问题:**
```cpp
// ❌ 错误: 使用小写
class myClass {}

// ✅ 正确: 使用大驼峰
class MyClass {}
```

### ✅ 函数命名
- [ ] 函数使用小驼峰命名法
- [ ] 访问器使用get/set前缀
- [ ] 布尔返回值使用is/has前缀
- [ ] 函数名使用动词描述动作

### ✅ 变量命名
- [ ] 成员变量命名一致(m_前缀或下写下划线)
- [ ] 局部变量使用小写下划线
- [ ] 常量使用全大写下划线
- [ ] 变量名描述性强,避免单字母(除循环变量)

### ✅ 枚举命名
- [ ] 枚举类型使用大驼峰
- [ ] 枚举值命名一致(全大写或小驼峰)

---

## 内存管理检查

### ✅ 对象树使用
- [ ] new创建的QObject对象指定了父对象
- [ ] 父子关系清晰,避免循环依赖
- [ ] 没有手动delete有父对象的对象

**示例问题:**
```cpp
// ❌ 错误: 未指定父对象
QPushButton *button = new QPushButton();

// ✅ 正确: 指定父对象
QPushButton *button = new QPushButton(parent);

// ❌ 错误: 手动删除有父对象的对象
delete button; // button已有父对象
```

### ✅ 智能指针使用
- [ ] 独占所有权使用QScopedPointer
- [ ] 共享所有权使用QSharedPointer
- [ ] 避免循环引用(使用QWeakPointer)
- [ ] 原始指针优先使用Qt对象树

### ✅ 内存泄漏风险
- [ ] new和delete配对
- [ ] 没有未释放的动态内存
- [ ] 析构函数正确释放资源

---

## 信号槽检查

### ✅ 连接方式
- [ ] 优先使用函数指针连接(Qt 5.7+)
- [ ] 避免使用SIGNAL/SLOT宏
- [ ] 断开不需要的连接

**示例问题:**
```cpp
// ❌ 错误: 使用旧式宏(无类型安全)
connect(sender, SIGNAL(valueChanged(QString)),
        receiver, SLOT(updateValue(QString)));

// ✅ 正确: 使用函数指针
connect(sender, &Sender::valueChanged,
        receiver, &Receiver::updateValue);
```

### ✅ 信号槽参数
- [ ] 信号参数数量<=槽函数参数数量
- [ ] 自定义类型注册为元类型
- [ ] 跨线程连接使用正确的连接类型

### ✅ 信号命名
- [ ] 信号使用过去时或描述性动词
- [ ] 信号名表示状态改变
- [ ] 不在信号中执行耗时操作

---

## 线程安全检查

### ✅ 线程使用
- [ ] GUI操作只在主线程执行
- [ ] 跨线程通信使用信号槽
- [ ] 共享数据使用互斥锁保护

**示例问题:**
```cpp
// ❌ 错误: 在工作线程中操作GUI
void WorkerThread::run() {
    label->setText("Update"); // 错误!
}

// ✅ 正确: 通过信号槽更新GUI
connect(worker, &Worker::updateText,
        label, &QLabel::setText);
```

### ✅ 线程同步
- [ ] 使用QMutex保护共享数据
- [ ] 避免死锁(锁的顺序一致)
- [ ] 考虑使用QReadWriteLock提高并发

### ✅ QThread使用
- [ ] 正确使用moveToThread模式
- [ ] 线程正确清理和退出
- [ ] 避免在子线程操作GUI对象

---

## 性能优化检查

### ✅ 不必要的拷贝
- [ ] 大对象使用const引用传递
- [ ] 避免不必要的容器拷贝
- [ ] 使用QStringBuilder拼接字符串

**示例问题:**
```cpp
// ❌ 错误: 值传递大对象
void processList(QList<QString> items) {}

// ✅ 正确: const引用传递
void processList(const QList<QString>& items) {}
```

### ✅ 绘制优化
- [ ] 只重绘需要更新的区域
- [ ] 避免在paintEvent中复杂计算
- [ ] 使用QPixmap缓存复杂图形

### ✅ 延迟初始化
- [ ] 非关键资源延迟初始化
- [ ] 使用智能指针实现延迟初始化
- [ ] 避免构造函数中耗时操作

### ✅ 字符串处理
- [ ] 使用QStringLiteral而非""_QLatin1String
- [ ] 避免在循环中进行字符串转换
- [ ] 使用QStringView避免临时对象(Qt 5.10+)

---

## 代码风格检查

### ✅ 头文件组织
- [ ] 头文件包含保护(#ifndef)
- [ ] 包含顺序正确(对应头→Qt→第三方→项目)
- [ ] 使用前向声明减少依赖

**示例问题:**
```cpp
// ✅ 正确的包含顺序
#include "myclass.h"           // 1. 对应头文件
#include <QWidget>             // 2. Qt头文件
#include <thirdparty/lib.h>    // 3. 第三方库
#include "utils/helper.h"      // 4. 项目内头文件

// ✅ 使用前向声明
class QWidget; // 在.h中前向声明
#include <QWidget> // 在.cpp中包含
```

### ✅ 格式规范
- [ ] 缩进一致(建议4空格)
- [ ] 大括号位置一致
- [ ] 运算符周围有空格
- [ ] 代码行长度合理(建议<100字符)

### ✅ 注释质量
- [ ] 复杂逻辑有注释说明
- [ ] 公共API有文档注释
- [ ] 注释与代码一致
- [ ] 避免无意义的注释

---

## 架构设计检查

### ✅ 职责分离
- [ ] 类职责单一
- [ ] 高内聚低耦合
- [ ] 合理使用MVC/MVP模式

### ✅ 继承与组合
- [ ] 优先使用组合而非继承
- [ ] 虚函数适当使用
- [ ] 避免过深的继承层次

### ✅ 接口设计
- [ ] 接口简洁清晰
- [ ] 遵循最小知识原则
- [ ] 错误处理机制合理

---

## Qt特定规范检查

### ✅ 容器使用
- [ ] 选择合适的Qt容器类型
- [ ] 理解QList在Qt5/Qt6的差异
- [ ] 利用隐式共享(写时复制)

**Qt容器选择指南:**
- `QVector`: 连续内存,默认选择
- `QList`: Qt5中基于指针,Qt6中与QVector相同
- `QLinkedList`: 频繁插入删除
- `QMap/QHash`: 根据是否需要排序选择

### ✅ Qt特性使用
- [ ] 使用信号槽机制而非回调
- [ ] 利用Qt元对象系统
- [ ] 合理使用属性系统(Q_PROPERTY)

### ✅ 资源管理
- [ ] 使用Qt资源系统(.qrc)
- [ ] 正确关闭文件和设备
- [ ] 重写closeEvent处理清理

### ✅ 国际化支持
- [ ] 用户可见字符串使用tr()
- [ ] 提供字符串上下文
- [ ] 避免字符串拼接后再翻译

**示例问题:**
```cpp
// ❌ 错误: 字符串拼接
QString msg = tr("File") + " " + fileName + " " + tr("not found");

// ✅ 正确: 使用参数化字符串
QString msg = tr("File %1 not found").arg(fileName);
```

---

## 优先级分类

### 🔴 严重问题(Must Fix)
- 内存泄漏
- 线程安全问题
- GUI在非主线程操作
- 资源未释放

### 🟡 重要问题(Should Fix)
- 性能问题
- 信号槽连接错误
- 内存管理不当
- 明显的架构缺陷

### 🟢 建议改进(Nice to Have)
- 命名规范
- 代码风格
- 注释质量
- 可选的性能优化

---

## 审查流程

1. **整体浏览**: 理解代码结构和目的
2. **逐项检查**: 按优先级检查各项
3. **记录问题**: 标注位置、类型、建议
4. **总结反馈**: 提供改进建议和优先级

---

## 常见反模式

### ❌ 反模式1: 内存管理混乱
```cpp
// 反模式: new/delete配对错误
obj = new MyClass();
obj = new MyClass(); // 内存泄漏!
```

### ❌ 反模式2: 错误的信号槽连接
```cpp
// 反模式: 跨线程使用DirectConnection
connect(worker, &Worker::progress,
        this, &MyClass::updateProgress,
        Qt::DirectConnection); // 可能崩溃!
```

### ❌ 反模式3: 不必要的拷贝
```cpp
// 反模式: 循环中拷贝容器
for (auto item : bigList) {
    // item是拷贝!
}
// 应该使用: const auto& item
```

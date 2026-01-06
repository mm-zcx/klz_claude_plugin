# Qt编码规范参考

## 1. 命名规范

### 1.1 类命名
- 使用大驼峰命名法(CamelCase): `class MyClass {}`
- 类名使用名词,避免使用动词
- 抽象类以抽象概念命名,或以Interface/Base结尾

### 1.2 函数和方法命名
- 使用小驼峰命名法: `void myFunction() {}`
- 访问器以get/set前缀: `getValue()`, `setValue()`
- 布尔返回值函数以is/has开头: `isValid()`, `hasFeature()`

### 1.3 变量命名
- 成员变量使用m_前缀或小写下划线: `m_value` 或 `myValue`
- 局部变量使用小写下划线命名: `myVariable`
- 常量使用全大写下划线: `const int MAX_COUNT = 100;`

### 1.4 枚举命名
- 枚举类型使用大驼峰: `enum ColorType {}`
- 枚举值使用全大写下划线或小驼峰: `RED`, `Blue`

## 2. 信号与槽

### 2.1 信号命名
- 使用过去时或动词形式: `void changed()`, `void valueChanged()`
- 信号函数名应该是描述性的,表示状态改变

### 2.2 槽函数命名
- 槽函数可以使用on...模式: `void onButtonClicked()`
- 或者使用动词形式描述动作: `void handleEvent()`

### 2.3 连接规范
```cpp
// 推荐: 使用函数指针(Qt 5.7+)
connect(sender, &Sender::valueChanged, receiver, &Receiver::updateValue);

// 避免旧式的SIGNAL/SLOT宏(运行时检查,无类型安全)
connect(sender, SIGNAL(valueChanged(QString)), receiver, SLOT(updateValue(QString)));
```

## 3. 内存管理

### 3.1 对象树
- Qt对象树自动管理子对象内存
- 父对象析构时自动析构所有子对象
```cpp
// 正确: 指定父对象
QWidget *widget = new QWidget(parent);
QPushButton *button = new QPushButton(widget);
```

### 3.2 智能指针
- 使用QScopedPointer管理独占所有权
- 使用QSharedPointer管理共享所有权
- 使用QWeakPointer避免循环引用
```cpp
// 推荐: 使用QScopedPointer
QScopedPointer<QFile> file(new QFile("data.txt"));
```

### 3.3 内存泄漏检查
- 确保new和delete配对
- 优先使用Qt对象树而非手动delete
- 注意信号槽连接可能导致隐式共享

## 4. Qt容器使用

### 4.1 容器选择
- `QVector`: 连续内存,适合大多数场景
- `QList`: 基于指针数组,适合大对象(Qt5)或统一接口(Qt6)
- `QLinkedList`: 链表结构,频繁插入删除场景
- `QMap`: 有序映射表
- `QHash`: 哈希表,更快的查找

### 4.2 隐式共享(COW)
- Qt容器使用隐式共享(写时复制)
- 传递容器时使用const引用避免不必要的拷贝
```cpp
// 正确: 使用const引用
void processList(const QList<QString>& items);

// 避免不必要的detach()
```

## 5. 信号槽参数

### 5.1 参数传递
- 信号参数使用值传递(利用隐式共享)
- 自定义类型需要使用qRegisterMetaType注册
```cpp
qRegisterMetaType<MyCustomType>("MyCustomType");
```

### 5.2 连接类型
- AutoConnection: 默认,根据发送者线程自动选择
- DirectConnection: 直接调用,同线程
- QueuedConnection: 事件队列,跨线程
- BlockingQueuedConnection: 阻塞等待,跨线程同步

## 6. 线程安全

### 6.1 QThread使用
- 子类化QThread并重写run()方法
- 或使用moveToThread将对象移到工作线程
```cpp
// 推荐: 使用moveToThread
QThread *thread = new QThread;
Worker *worker = new Worker;
worker->moveToThread(thread);
```

### 6.2 线程安全
- GUI操作只能在主线程进行
- 跨线程通信使用信号槽(QueuedConnection)
- 使用QMutex、QReadWriteLock保护共享数据
- 使用QAtomicInt、QAtomicPointer进行原子操作

## 7. 资源管理

### 7.1 文件操作
- 使用QFile进行文件I/O
- 确保文件正确关闭
```cpp
QFile file("data.txt");
if (file.open(QIODevice::ReadOnly)) {
    // 操作文件
    file.close();
}
```

### 7.2 资源释放
- 在析构函数中释放资源
- 重写closeEvent处理窗口关闭
- 使用RAII模式管理资源

## 8. 编码规范

### 8.1 头文件保护
```cpp
#ifndef MYCLASS_H
#define MYCLASS_H

// 类声明

#endif // MYCLASS_H
```

### 8.2 包含顺序
1. 对应的头文件
2. Qt头文件
3. 第三方库头文件
4. 项目内头文件

### 8.3 前向声明
- 使用前向声明减少头文件依赖
- 在.cpp文件中包含完整定义
```cpp
// .h文件中使用前向声明
class QWidget;

// .cpp文件中包含完整定义
#include <QWidget>
```

## 9. 性能优化

### 9.1 避免不必要的拷贝
- 使用const引用传递大对象
- 利用Qt的隐式共享机制
- 使用QStringBuilder进行字符串拼接

### 9.2 延迟初始化
- 使用std::unique_ptr或QScopedPointer延迟初始化
- 实现lazy loading模式

### 9.3 绘制优化
- 只重绘需要更新的区域
- 使用QPixmap缓存复杂图形
- 避免在paintEvent中进行复杂计算

## 10. Qt版本兼容性

### 10.1 Qt5到Qt6迁移
- QList变化: Qt6中QList基于QVector实现
- QRegExp替换为QRegularExpression
- QIcon::fromTheme需要实际主题支持

### 10.2 版本检测
```cpp
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6代码
#else
    // Qt5代码
#endif
```

## 11. 错误处理

### 11.1 错误检查
- 检查函数返回值
- 使用qWarning()、qCritical()记录错误
- 避免使用std::exception(Qt不处理)

### 11.2 断言
- 使用Q_ASSERT进行调试时断言
- 使用Q_CHECK_PTR检查指针
- 发布版禁用断言

## 12. 国际化

### 12.1 可翻译字符串
```cpp
// 使用tr()包裹用户可见字符串
QString text = tr("Hello, World!");

// 提供上下文
QString status = tr("Status", "File status");
```

### 12.2 字符串处理
- 使用QString而非std::string
- 使用QStringLiteral编译期字符串
- 避免在循环中进行字符串转换

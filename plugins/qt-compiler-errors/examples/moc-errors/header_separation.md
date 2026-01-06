# 头文件分离的最佳实践

## 问题：MOC编译器与代码结构

在Qt项目中，MOC（Meta-Object Compiler）需要能够访问所有包含`Q_OBJECT`宏的类定义。当类定义和实现混合在一起时，常常会导致编译错误。

## 最佳实践：头文件分离

### 1. 标准的Qt类结构

```cpp
// myclass.h
#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QTimer>

class MyClass : public QObject {
    Q_OBJECT  // 宏声明在头文件中

public:
    explicit MyClass(QObject *parent = nullptr);

signals:
    void dataProcessed(const QByteArray &result);
    void statusChanged(bool isRunning);

public slots:
    void startProcessing();
    void stopProcessing();

private slots:
    void onTimeout();

private:
    void processData();

    QTimer *m_timer;
    bool m_isRunning;
    QByteArray m_data;
};

#endif // MYCLASS_H
```

```cpp
// myclass.cpp
#include "myclass.h"

MyClass::MyClass(QObject *parent)
    : QObject(parent), m_timer(new QTimer(this)), m_isRunning(false) {

    connect(m_timer, &QTimer::timeout, this, &MyClass::onTimeout);
}

void MyClass::startProcessing() {
    m_isRunning = true;
    m_timer->start(1000);  // 1秒间隔
    emit statusChanged(true);
}

void MyClass::stopProcessing() {
    m_isRunning = false;
    m_timer->stop();
    emit statusChanged(false);
}

void MyClass::onTimeout() {
    processData();
}

void MyClass::processData() {
    // 处理数据逻辑
    QByteArray result;
    // ... 数据处理 ...
    emit dataProcessed(result);
}
```

### 2. 避免的错误模式

❌ **错误：将整个类定义放在cpp文件中**
```cpp
// 错误示例 - 不要这样做！
// myclass.cpp
#include <QObject>

class MyBadClass : public QObject {
    Q_OBJECT  // MOC可能找不到这个宏！
public:
    MyBadClass() {}
};
```

❌ **错误：手动包含.moc文件**
```cpp
// myclass.cpp
#include "myclass.h"

// ... 类的实现 ...

#include "moc_myclass.cpp"  // 不需要手动包含！
```

### 3. MOC编译器的工作原理

1. **头文件扫描**：MOC扫描所有头文件中的`Q_OBJECT`宏
2. **元对象代码生成**：为每个包含`Q_OBJECT`的类生成元对象代码
3. **自动编译**：构建系统自动编译生成的moc文件
4. **链接**：将元对象代码链接到最终的可执行文件中

### 4. 构建系统集成

#### qmake系统
- qmake自动检测需要MOC处理的文件
- 自动生成`moc_*.cpp`文件
- 自动编译和链接MOC输出

#### CMake系统
```cmake
set(CMAKE_AUTOMOC ON)  # 启用自动MOC处理

# 自动扫描源文件中的Q_OBJECT宏
file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(myapp ${SOURCES})
```

### 5. 调试MOC问题

#### 常见错误信息
- `undefined reference to 'vtable for ClassName'`
- `No rule to make target 'moc_ClassName.cpp'`
- `collect2: error: ld returned 1 exit status`

#### 诊断步骤
1. 确认所有`QObject`派生类都有`Q_OBJECT`宏
2. 检查头文件是否被正确包含
3. 清理构建目录并重新编译
4. 验证构建配置（AUTOMOC是否启用）

### 6. 特殊情况处理

#### 条件编译
```cpp
// 条件编译时的MOC处理
#ifdef FEATURE_ENABLED
class FeatureClass : public QObject {
    Q_OBJECT
public:
    FeatureClass(QObject *parent = nullptr) : QObject(parent) {}
signals:
    void featureActivated();
};
#else
class FeatureClass : public QObject {
    Q_OBJECT  // 保持宏以确保MOC一致性
public:
    FeatureClass(QObject *parent = nullptr) : QObject(parent) {}
signals:
    void featureActivated();  // 保持信号接口
};
#endif
```

#### 模板类与Qt
```cpp
// 模板类不能使用Q_OBJECT宏
template<typename T>
class MyTemplate {  // 不能继承QObject
    // 模板类中不能使用Qt的元对象系统
public:
    void processData(const T& data) {
        // 普通C++代码
    }
};

// 如果需要Qt功能，使用非模板的包装类
class MyWrapper : public QObject {
    Q_OBJECT
public:
    template<typename T>
    void processTemplateData(const T& data) {
        // 处理模板数据并发送信号
        emit dataReady();
    }

signals:
    void dataReady();
};
```

### 7. 清理和重建

当遇到MOC相关问题时，标准的解决流程是：

```bash
# 清理所有构建文件
rm -rf build/
# 或者
make clean

# 重新运行配置
qmake  # 或 cmake ..
make   # 或 ninja/cmake --build
```

在Qt Creator中：
1. Build → Clean All
2. Build → Run qmake
3. Build → Rebuild All
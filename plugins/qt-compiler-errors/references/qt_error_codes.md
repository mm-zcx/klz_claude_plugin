# Qt编译错误代码参考

## MOC (Meta-Object Compiler) 错误

### vtable错误
```
undefined reference to 'vtable for MyClass'
```
**原因**: 缺少Q_OBJECT宏或类没有正确继承QObject
**解决**: 确保类继承QObject并包含Q_OBJECT宏

### MOC生成错误
```
No rule to make target 'MyClass.moc'
```
**原因**: 构建系统无法生成MOC文件
**解决**: 清理并重新构建项目

## 信号槽连接错误

### 过时信号错误
```
error: 'error' is deprecated
```
**原因**: 使用了过时的信号名
**解决方案**:
```cpp
// Qt 5.15+
QAbstractSocket::errorOccurred

// Qt 5.15之前
QAbstractSocket::error
```

### 参数类型不匹配
```
error: no matching function for call to 'connect'
```
**原因**: 信号和槽的参数类型不匹配
**解决**: 确保参数类型完全匹配

## 构建系统错误

### CMake错误

#### 找不到Qt5包
```
CMake Error: Could not find a package configuration file provided by "Qt5"
```
**解决**:
```bash
sudo apt-get install qtbase5-dev qt5-default
```

#### 缺少Qt模块
```
error: 'QNetwork' was not declared in this scope
```
**解决**: 在CMakeLists.txt中添加相应模块
```cmake
find_package(Qt5 REQUIRED COMPONENTS Core Network)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)
```

### qmake错误

#### 缺少Qt模块
```
error: QNetwork: No such file or directory
```
**解决**: 在.pro文件中添加模块
```pro
QT += core network
```

## 类型转换错误

### quint16范围检查警告
```
warning: comparison is always true due to limited range of data type
```
**原因**: quint16的最大值已经是65535
**解决**: 移除不必要的上界检查
```cpp
// 错误
bool isValidPort(quint16 port) {
    return port >= 1024 && port <= 65535;
}

// 正确
bool isValidPort(quint16 port) {
    return port >= 1024;
}
```

### 未使用参数警告
```
warning: unused parameter 'param'
```
**解决**: 使用Q_UNUSED宏
```cpp
void myFunction(int param) {
    Q_UNUSED(param);
}
```

## 链接错误

### 找不到符号
```
undefined reference to 'ClassName::methodName'
```
**原因**:
1. 忘记在CMakeLists.txt或.pro文件中包含源文件
2. 忘记链接库
3. 函数实现缺失

**解决方案**:
```cmake
# CMakeLists.txt
add_executable(${PROJECT_NAME} source1.cpp source2.cpp)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
```

```pro
# .pro文件
SOURCES += source1.cpp source2.cpp
LIBS += -lQt5Core
```

## Qt版本兼容性问题

### Qt4/Qt5 API变更

#### QString::ascii()
```cpp
// Qt4 (已过时)
QString str = myString.ascii();

// Qt5+
QString str = myString.toLatin1();  // 或 toUtf8()
```

#### QUrl转换
```cpp
// Qt4
QUrl url = QUrl::fromEncoded(myString.toAscii());

// Qt5+
QUrl url = QUrl::fromEncoded(myString.toLatin1());
```

### 构建器变更

#### moc命令变更
```bash
# Qt4
moc myclass.h -o moc_myclass.cpp

# Qt5 (自动处理)
# MOC文件由构建系统自动生成
```

## 最佳实践

### 1. 项目结构
```
project/
├── src/
│   ├── myclass.h     # 类声明
│   ├── myclass.cpp   # 类实现
│   └── main.cpp
├── CMakeLists.txt     # 或 .pro文件
└── build/            # 构建输出目录
```

### 2. 构建清理脚本
```bash
#!/bin/bash
echo "清理Qt构建文件..."
rm -rf build/
rm -f *.o moc_* qrc_* ui_*
make clean
qmake
make
```

### 3. 调试编译错误
```bash
# 使用详细输出
make VERBOSE=1

# 或在CMake中添加
set(CMAKE_VERBOSE_MAKEFILE ON)
```

## 常见问题诊断流程

### 1. MOC问题
1. 检查是否包含Q_OBJECT宏
2. 检查类是否继承QObject
3. 清理并重新构建

### 2. 信号槽问题
1. 检查信号和槽的参数类型
2. 检查Qt版本兼容性
3. 验证连接语法

### 3. 构建系统问题
1. 验证Qt安装
2. 检查CMakeLists.txt或.pro文件
3. 确认所有必要的模块都已添加

### 4. 链接问题
1. 检查源文件是否被包含
2. 验证库链接
3. 检查符号是否存在

## 有用的工具和命令

### Qt助手 (Qt Assistant)
```bash
assistant
```

### qmake工具
```bash
qmake -v          # 显示版本信息
qmake -project   # 从源文件生成.pro文件
```

### CMake诊断
```bash
cmake --version
cmake --debug-output
```

### 查找符号
```bash
nm libname.so | grep symbol_name
objdump -t object_file.o | grep symbol_name
```

## 参考资料

- [Qt官方文档](https://doc.qt.io/qt-5/)
- [CMake文档](https://cmake.org/documentation/)
- [Qt构建系统](https://doc.qt.io/qt-5/qmake-manual.html)
- [Qt信号槽](https://doc.qt.io/qt-5/signalsandslots.html)
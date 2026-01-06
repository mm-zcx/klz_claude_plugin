/**
 * @file vtable_errors.cpp
 * @brief MOC错误示例：vtable错误的解决方案
 *
 * 这个示例展示了vtable错误的常见原因和解决方案
 */

#include <QObject>

// 问题1：类定义在cpp文件中导致vtable错误
//
// 错误示例 - 不要这样做！
/*
// main.cpp
#include <QObject>

class ProblematicClass : public QObject {
    Q_OBJECT
public:
    ProblematicClass(QObject *parent = nullptr) : QObject(parent) {}
};

int main() {
    ProblematicClass obj;  // 可能导致vtable错误
}
*/

// 解决方案：将类定义分离到头文件和源文件中

// myclass.h
#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>

class MyProperClass : public QObject {
    Q_OBJECT
public:
    explicit MyProperClass(QObject *parent = nullptr);

signals:
    void initialized();

private slots:
    void onTimerTimeout();

private:
    QTimer *m_timer;
};

#endif // MYCLASS_H

// myclass.cpp
#include "myclass.h"
#include <QTimer>

MyProperClass::MyProperClass(QObject *parent)
    : QObject(parent), m_timer(new QTimer(this)) {

    connect(m_timer, &QTimer::timeout, this, &MyProperClass::onTimerTimeout);
    emit initialized();
}

void MyProperClass::onTimerTimeout() {
    // 定时器超时处理
}

// 问题2：条件编译中的Q_OBJECT
//
// 解决方案：确保MOC能处理所有条件分支
#ifdef HARDWARE_SUPPORT
class HardwareClass : public QObject {
    Q_OBJECT
public:
    explicit HardwareClass(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void hardwareReady();
};
#else
class HardwareClass : public QObject {
    Q_OBJECT  // 即使在没有硬件支持时也要保留
public:
    explicit HardwareClass(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void hardwareReady();  // 保持信号接口一致
};
#endif
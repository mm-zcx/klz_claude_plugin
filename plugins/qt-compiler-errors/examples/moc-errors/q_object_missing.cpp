/**
 * @file q_object_missing.cpp
 * @brief MOC错误示例：缺少Q_OBJECT宏
 *
 * 这个示例展示了当QObject派生类缺少Q_OBJECT宏时常见的编译错误
 */

#include <QObject>

// 错误示例：缺少Q_OBJECT宏
class MyBrokenClass : public QObject {
    // 缺少 Q_OBJECT 宏！

public:
    explicit MyBrokenClass(QObject *parent = nullptr) : QObject(parent) {}

    // 尝试定义信号，但没有Q_OBJECT宏会导致编译错误
signals:
    void dataReceived(const QByteArray &data);

public slots:
    void processIncomingData() {
        // 处理数据
    }
};

// 正确的示例：包含Q_OBJECT宏
class MyCorrectClass : public QObject {
    Q_OBJECT  // 必须的宏

public:
    explicit MyCorrectClass(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void dataReceived(const QByteArray &data);
    void statusChanged(bool isConnected);

public slots:
    void processIncomingData() {
        // 处理数据逻辑
        emit statusChanged(true);
    }

private:
    QByteArray m_buffer;
};
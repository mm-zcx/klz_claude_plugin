/**
 * @file parameter_mismatch.cpp
 * @brief Qt信号槽错误示例：参数类型不匹配
 *
 * 这个示例展示了信号槽参数类型匹配的常见问题和解决方案
 */

#include <QObject>
#include <QByteArray>
#include <QString>

class DataProcessor : public QObject {
    Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr) : QObject(parent) {}

signals:
    // 各种参数类型的信号声明
    void dataReceived(const QByteArray &data);
    void messageReceived(QString message);
    void progressUpdated(int percentage);
    void statusChanged(bool isReady);

public slots:
    // 接收数据的槽函数
    void handleData(const QByteArray &data) {
        if (data.isEmpty()) {
            qWarning() << "接收到空数据";
            return;
        }

        qDebug() << "处理" << data.size() << "字节的数据";
        // 处理数据逻辑...
    }

    // 错误示例：参数类型不匹配
    /*
    void handleMessage(QByteArray data) {  // ❌ 错误：信号是QString，槽是QByteArray
        // 这个槽不会被正确调用
    }
    */

    // 正确示例：参数类型匹配
    void handleMessage(const QString &message) {  // ✅ 正确：QString信号 → QString槽
        qDebug() << "收到消息:" << message;
    }

    // 或者使用隐式转换（如果支持）
    void handleMessageBytes(QByteArray messageBytes) {  // ✅ 也可以工作：QString → QByteArray 转换
        QString message = QString::fromUtf8(messageBytes);
        qDebug() << "收到消息(字节转字符串):" << message;
    }

    // 值传递 vs 引用传递
    void handleProgress(int percentage) {  // ✅ 值传递：完全匹配
        qDebug() << "进度:" << percentage << "%";
    }

    void handleProgressByReference(const int &percentage) {  // ✅ 引用传递：也匹配
        qDebug() << "进度(引用):" << percentage << "%";
    }

    // const正确性
    void handleStatus(bool status) {  // ✅ 值传递，忽略const
        qDebug() << "状态:" << (status ? "就绪" : "未就绪");
    }

    void handleStatusRef(const bool &status) {  // ✅ const引用传递
        qDebug() << "状态(引用):" << (status ? "就绪" : "未就绪");
    }

    // ❌ 错误：不能匹配
    // void handleStatusWrong(const bool *status) {  // 指针类型不匹配引用类型
    //     // 这会导致编译错误或运行时连接失败
    // }
};

// 参数匹配规则的详细示例
class ParameterMatchingExample : public QObject {
    Q_OBJECT

public:
    explicit ParameterMatchingExample(QObject *parent = nullptr);

public slots:
    void testSignalSlotConnections();

signals:
    void signalInt(int value);
    void signalIntRef(int &value);
    void signalIntConstRef(const int &value);
    void signalString(const QString &text);
    void signalByteArray(const QByteArray &data);

private slots:
    // 各种槽函数声明
    void slotInt(int value);
    void slotIntRef(int &value);
    void slotIntConstRef(const int &value);
    void slotIntConst(const int value);
    void slotString(QString text);
    void slotStringRef(QString &text);
    void slotStringConstRef(const QString &text);
    void slotStringConst(const QString &text);
};

ParameterMatchingExample::ParameterMatchingExample(QObject *parent) : QObject(parent) {
}

void ParameterMatchingExample::testSignalSlotConnections() {
    // ✅ 这些连接会成功
    connect(this, &ParameterMatchingExample::signalInt, this, &ParameterMatchingExample::slotInt);
    connect(this, &ParameterMatchingExample::signalInt, this, &ParameterMatchingExample::slotIntConst);
    connect(this, &ParameterMatchingExample::signalIntRef, this, &ParameterMatchingExample::slotInt);
    connect(this, &ParameterMatchingExample::signalIntRef, this, &ParameterMatchingExample::slotIntRef);
    connect(this, &ParameterMatchingExample::signalIntConstRef, this, &ParameterMatchingExample::slotInt);
    connect(this, &ParameterMatchingExample::signalIntConstRef, this, &ParameterMatchingExample::slotIntRef);
    connect(this, &ParameterMatchingExample::signalIntConstRef, this, &ParameterMatchingExample::slotIntConstRef);
    connect(this, &ParameterMatchingExample::signalIntConstRef, this, &ParameterMatchingExample::slotIntConst);

    // 字符串类型的连接
    connect(this, &ParameterMatchingExample::signalString, this, &ParameterMatchingExample::slotString);
    connect(this, &ParameterMatchingExample::signalString, this, &ParameterMatchingExample::slotStringConst);
    connect(this, &ParameterMatchingExample::signalString, this, &ParameterMatchingExample::slotStringConstRef);
    connect(this, &ParameterMatchingExample::signalStringRef, this, &ParameterMatchingExample::slotStringRef);

    // ❌ 这些连接会失败或导致警告
    /*
    connect(this, &ParameterMatchingExample::signalString, this, &ParameterMatchingExample::slotInt);  // 类型不匹配
    connect(this, &ParameterMatchingExample::signalInt, this, &ParameterMatchingExample::slotString);  // 类型不匹配
    */
}

// 实现槽函数
void ParameterMatchingExample::slotInt(int value) {
    qDebug() << "slotInt:" << value;
}

void ParameterMatchingExample::slotIntRef(int &value) {
    qDebug() << "slotIntRef:" << value;
    value += 1;  // 修改原始值
}

void ParameterMatchingExample::slotIntConstRef(const int &value) {
    qDebug() << "slotIntConstRef:" << value;
    // 不能修改value，是const引用
}

void ParameterMatchingExample::slotIntConst(const int value) {
    qDebug() << "slotIntConst:" << value;
}

void ParameterMatchingExample::slotString(QString text) {
    qDebug() << "slotString:" << text;
}

void ParameterMatchingExample::slotStringRef(QString &text) {
    qDebug() << "slotStringRef:" << text;
    text += " [modified]";  // 修改原始字符串
}

void ParameterMatchingExample::slotStringConstRef(const QString &text) {
    qDebug() << "slotStringConstRef:" << text;
    // 不能修改text，是const引用
}

void ParameterMatchingExample::slotStringConst(const QString &text) {
    qDebug() << "slotStringConst:" << text;
}
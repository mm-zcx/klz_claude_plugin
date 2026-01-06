/**
 * @file deprecated_signals.cpp
 * @brief Qt信号槽错误示例：已弃用的信号处理
 *
 * 这个示例展示了如何处理Qt版本升级中的弃用信号问题
 */

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>

class NetworkClient : public QObject {
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);

public slots:
    void connectToHost(const QString &host, quint16 port);
    void handleSocketError();

private:
    QTcpSocket *m_socket;
};

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent), m_socket(new QTcpSocket(this)) {

    // ❌ 错误：使用已弃用的QAbstractSocket::error信号 (Qt 5.15+)
    /*
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &NetworkClient::handleSocketError);
    */

    // ✅ 正确：使用新的errorOccurred信号 (Qt 5.15+)
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &NetworkClient::handleSocketError);
}

void NetworkClient::connectToHost(const QString &host, quint16 port) {
    m_socket->connectToHost(host, port);
}

void NetworkClient::handleSocketError() {
    QAbstractSocket::SocketError error = m_socket->error();
    QString errorString = m_socket->errorString();

    qCritical() << "Socket error:" << error << errorString;

    // 根据错误类型进行不同的处理
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "连接被拒绝";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "主机未找到";
        break;
    case QAbstractSocket::NetworkError:
        qDebug() << "网络错误";
        break;
    default:
        qDebug() << "其他错误:" << errorString;
        break;
    }
}

// 更多弃用信号的示例
class DeprecatedSignalExample : public QObject {
    Q_OBJECT

public:
    DeprecatedSignalExample(QObject *parent = nullptr);

private slots:
    void handleButtonClicked();

private:
    // 模拟一个已弃用的按钮类
    class DeprecatedButton {
    public:
        // 旧版本的信号
        // signal void clicked();  // 已弃用

        // 新版本的信号
        // signal void clicked(bool checked);  // 推荐使用
    };
};

// 对于自定义信号，推荐使用更好的命名和参数
class ModernSignalExample : public QObject {
    Q_OBJECT

public:
    explicit ModernSignalExample(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void processData();

signals:
    // 推荐的信号命名：使用动词开头，描述性参数
    void dataProcessingStarted();
    void dataProcessingCompleted(const QByteArray &result);
    void dataProcessingFailed(const QString &errorMessage);

    // 避免的命名方式
    // void data();  // 太模糊
    // void error();  // 不够具体

    // 推荐的布尔参数命名
    void connectionStateChanged(bool isConnected);  // 比单纯的 connected() 更好
    void validationPassed(bool isValid);  // 比单纯的 validated() 更明确
};
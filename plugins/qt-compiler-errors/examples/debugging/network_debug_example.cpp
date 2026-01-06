/**
 * @file network_debug_example.cpp
 * @brief Qt网络编程调试示例代码
 *
 * 这个示例展示了如何为网络服务器添加全面的调试日志
 */

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>
#include <QDebug>

class DebuggableNetworkServer : public QTcpServer {
    Q_OBJECT

public:
    explicit DebuggableNetworkServer(QObject *parent = nullptr);

    bool startServer(quint16 port, int droneId = 1);

public slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();
    void handleSocketError(QAbstractSocket::SocketError error);

private slots:
    void printDebugInfo();

private:
    void logConnectionInfo(const QString &message, QTcpSocket *socket = nullptr);
    void logDataInfo(const QString &message, const QByteArray &data = QByteArray());

    QList<QTcpSocket*> m_clients;
    QMap<QTcpSocket, quint32> m_clientIds;
    quint32 m_nextClientId;
    int m_droneId;
    QTimer m_debugTimer;
};

DebuggableNetworkServer::DebuggableNetworkServer(QObject *parent)
    : QTcpServer(parent), m_nextClientId(1), m_droneId(0) {

    qDebug() << "[DEBUG] DebuggableNetworkServer constructor called";
    qDebug() << "[DEBUG] Server thread:" << QThread::currentThread();

    // 设置调试定时器，每5秒打印一次状态信息
    m_debugTimer.setInterval(5000);
    connect(&m_debugTimer, &QTimer::timeout, this, &DebuggableNetworkServer::printDebugInfo);
}

bool DebuggableNetworkServer::startServer(quint16 port, int droneId) {
    m_droneId = droneId;

    logConnectionInfo(QString("尝试启动服务器，端口: %1, 无人机ID: %2").arg(port).arg(droneId));

    // 连接信号（在listen()之前连接）
    connect(this, &QTcpServer::newConnection,
            this, &DebuggableNetworkServer::handleNewConnection, Qt::QueuedConnection);

    logConnectionInfo("信号槽连接已建立，准备监听");

    // 开始监听
    if (!listen(QHostAddress::Any, port)) {
        logConnectionInfo(QString("监听失败: %1").arg(errorString()));
        qCritical() << "[ERROR] Failed to start server on port" << port << ":" << errorString();
        return false;
    }

    logConnectionInfo(QString("服务器成功启动，监听地址: %1:%2")
                     .arg(serverAddress().toString())
                     .arg(serverPort()));

    // 启动调试定时器
    m_debugTimer.start();

    return true;
}

void DebuggableNetworkServer::handleNewConnection() {
    qDebug() << "[DEBUG] handleNewConnection() called!";
    qDebug() << "[DEBUG] Current thread:" << QThread::currentThread();
    qDebug() << "[DEBUG] Pending connections count:" << hasPendingConnections();

    while (hasPendingConnections()) {
        QTcpSocket *socket = nextPendingConnection();
        if (!socket) {
            qDebug() << "[DEBUG] nextPendingConnection returned null";
            continue;
        }

        quint32 clientId = m_nextClientId++;
        m_clients.append(socket);
        m_clientIds[socket] = clientId;

        // 连接客户端信号
        connect(socket, &QTcpSocket::readyRead,
                this, &DebuggableNetworkServer::handleReadyRead, Qt::QueuedConnection);
        connect(socket, &QTcpSocket::disconnected,
                this, &DebuggableNetworkServer::handleDisconnected, Qt::QueuedConnection);
        connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
                this, &DebuggableNetworkServer::handleSocketError);

        logConnectionInfo(QString("新的客户端连接，客户端ID: %1").arg(clientId), socket);

        // 发送欢迎消息
        QString welcomeMsg = QString("欢迎使用网络服务器！客户端ID: %1\\n").arg(clientId);
        socket->write(welcomeMsg.toUtf8());

        qDebug() << "[DEBUG] 已发送欢迎消息给客户端" << clientId;
    }
}

void DebuggableNetworkServer::handleReadyRead() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qDebug() << "[DEBUG] handleReadyRead() called but sender is not QTcpSocket";
        return;
    }

    quint32 clientId = m_clientIds.value(client, 0);
    qDebug() << "[DEBUG] handleReadyRead() called for client" << clientId;

    QByteArray data = client->readAll();
    if (data.isEmpty()) {
        qDebug() << "[DEBUG] No data available for client" << clientId;
        return;
    }

    logDataInfo(QString("接收数据，客户端ID: %1").arg(clientId), data);

    // 处理数据（简单的回显）
    QString response = QString("收到数据 [%1]: %2\\n")
                      .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                      .arg(QString::fromUtf8(data).trimmed());

    client->write(response.toUtf8());

    logDataInfo(QString("发送响应，客户端ID: %1").arg(clientId), response.toUtf8());
}

void DebuggableNetworkServer::handleDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qDebug() << "[DEBUG] handleDisconnected() called but sender is not QTcpSocket";
        return;
    }

    quint32 clientId = m_clientIds.value(client, 0);
    logConnectionInfo(QString("客户端断开连接，客户端ID: %1").arg(clientId), client);

    m_clients.removeAll(client);
    m_clientIds.remove(client);

    qDebug() << "[DEBUG] Current connected clients count:" << m_clients.size();

    client->deleteLater();
}

void DebuggableNetworkServer::handleSocketError(QAbstractSocket::SocketError error) {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qDebug() << "[DEBUG] handleSocketError() called but sender is not QTcpSocket";
        return;
    }

    quint32 clientId = m_clientIds.value(client, 0);

    QString errorString;
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        errorString = "连接被拒绝";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorString = "远程主机关闭连接";
        break;
    case QAbstractSocket::HostNotFoundError:
        errorString = "主机未找到";
        break;
    case QAbstractSocket::SocketTimeoutError:
        errorString = "连接超时";
        break;
    case QAbstractSocket::NetworkError:
        errorString = "网络错误";
        break;
    default:
        errorString = client->errorString();
        break;
    }

    qCritical() << "[ERROR] 客户端" << clientId << "发生错误:" << errorString;
    logConnectionInfo(QString("客户端错误: %1").arg(errorString), client);
}

void DebuggableNetworkServer::printDebugInfo() {
    qDebug() << "[DEBUG] === 服务器状态信息 ===";
    qDebug() << "[DEBUG] 服务器运行状态:" << (isListening() ? "运行中" : "已停止");
    qDebug() << "[DEBUG] 监听端口:" << serverPort();
    qDebug() << "[DEBUG] 连接的客户端数量:" << m_clients.size();

    if (!m_clients.isEmpty()) {
        qDebug() << "[DEBUG] 客户端详细信息:";
        for (int i = 0; i < m_clients.size(); ++i) {
            QTcpSocket *client = m_clients[i];
            quint32 clientId = m_clientIds.value(client, 0);
            qDebug() << "[DEBUG]   客户端" << clientId << ":"
                     << client->peerAddress().toString()
                     << ":" << client->peerPort()
                     << "状态:" << client->state();
        }
    }

    qDebug() << "[DEBUG] 线程信息:" << QThread::currentThread();
    qDebug() << "[DEBUG] 当前时间:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug() << "[DEBUG] ========================";
}

void DebuggableNetworkServer::logConnectionInfo(const QString &message, QTcpSocket *socket) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString logMessage;

    if (socket) {
        logMessage = QString("[%1] [CONNECTION] %2 - 客户端 %3:%4 (ID:%5)")
                    .arg(timestamp)
                    .arg(message)
                    .arg(socket->peerAddress().toString())
                    .arg(socket->peerPort())
                    .arg(m_clientIds.value(socket, 0));
    } else {
        logMessage = QString("[%1] [CONNECTION] %2").arg(timestamp).arg(message);
    }

    qDebug() << logMessage;
}

void DebuggableNetworkServer::logDataInfo(const QString &message, const QByteArray &data) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    if (!data.isEmpty()) {
        // 创建数据的可读表示
        QString dataStr;
        if (data.size() > 64) {
            dataStr = QString("数据大小: %1 字节 (前64字节: %2)")
                     .arg(data.size())
                     .arg(QString::fromUtf8(data.left(64)).toHex());
        } else {
            dataStr = QString("数据: %1").arg(QString::fromUtf8(data).toHex());
        }

        qDebug() << QString("[%1] [DATA] %2 - %3").arg(timestamp).arg(message).arg(dataStr);
    } else {
        qDebug() << QString("[%1] [DATA] %2 - (无数据)").arg(timestamp).arg(message);
    }
}

// 使用示例
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // 设置调试输出格式
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] [%{type}] %{function}(): %{message}");

    qDebug() << "[MAIN] 应用程序启动";

    DebuggableNetworkServer server;

    // 尝试启动服务器
    quint16 port = 50001;
    if (!server.startServer(port, 1)) {
        qCritical() << "[MAIN] 服务器启动失败，退出应用程序";
        return 1;
    }

    qDebug() << "[MAIN] 服务器启动成功，应用程序运行中...";

    return app.exec();
}

#include "network_debug_example.moc"
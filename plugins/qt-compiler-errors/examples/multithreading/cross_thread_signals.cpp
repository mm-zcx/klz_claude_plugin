/**
 * @file cross_thread_signals.cpp
 * @brief Qt多线程信号槽错误示例：跨线程通信问题
 *
 * 这个示例展示了多线程环境中信号槽连接的常见问题和解决方案
 */

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

// 错误示例：跨线程信号槽连接问题
class BadNetworkServer : public QObject {
    Q_OBJECT

public:
    explicit BadNetworkServer(QObject *parent = nullptr);

public slots:
    void startServer();
    void handleNewConnection();

private:
    QTcpServer *m_server;
    QList<QTcpSocket*> m_clients;
};

BadNetworkServer::BadNetworkServer(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)) {

    // ❌ 错误：在主线程中连接，但信号可能在其他线程中发射
    connect(m_server, &QTcpServer::newConnection,
            this, &BadNetworkServer::handleNewConnection);  // 缺少Qt::QueuedConnection
}

void BadNetworkServer::startServer() {
    if (!m_server->listen(QHostAddress::Any, 50000)) {
        qCritical() << "Failed to start server:" << m_server->errorString();
        return;
    }

    qDebug() << "Server started on port 50000";
}

void BadNetworkServer::handleNewConnection() {
    qDebug() << "New connection received";

    while (m_server->hasPendingConnections()) {
        QTcpSocket *client = m_server->nextPendingConnection();
        if (!client) {
            continue;
        }

        m_clients.append(client);
        qDebug() << "Client connected:" << client->peerAddress().toString();

        // 连接客户端信号（在主线程中，但客户端可能在其他线程）
        connect(client, &QTcpSocket::readyRead,
                this, [client]() {
                    // ❌ 这可能在线程错误的上下文中执行
                    QByteArray data = client->readAll();
                    qDebug() << "Received data:" << data.toHex();
                });
    }
}

// 正确示例：使用Qt::QueuedConnection进行跨线程通信
class GoodNetworkServer : public QObject {
    Q_OBJECT

public:
    explicit GoodNetworkServer(QObject *parent = nullptr);

public slots:
    void startServer();
    void handleNewConnection();
    void handleClientDisconnected();
    void processClientData();

signals:
    void clientConnected(QTcpSocket *client);
    void clientDataReady(QTcpSocket *client, QByteArray data);

private:
    QTcpServer *m_server;
    QList<QTcpSocket*> m_clients;
    QMap<QTcpSocket*, QByteArray> m_clientBuffers;
};

GoodNetworkServer::GoodNetworkServer(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)) {

    // ✅ 正确：使用Qt::QueuedConnection确保跨线程安全
    connect(m_server, &QTcpServer::newConnection,
            this, &GoodNetworkServer::handleNewConnection, Qt::QueuedConnection);
}

void GoodNetworkServer::startServer() {
    qDebug() << "[DEBUG] Starting server in thread:" << QThread::currentThread();

    if (!m_server->listen(QHostAddress::Any, 50001)) {
        qCritical() << "Failed to start server:" << m_server->errorString();
        return;
    }

    qDebug() << "Server started on port 50001";
}

void GoodNetworkServer::handleNewConnection() {
    qDebug() << "[DEBUG] handleNewConnection called in thread:" << QThread::currentThread();

    while (m_server->hasPendingConnections()) {
        QTcpSocket *client = m_server->nextPendingConnection();
        if (!client) {
            continue;
        }

        m_clients.append(client);
        m_clientBuffers[client] = QByteArray();

        qDebug() << "[DEBUG] Client connected from:"
                 << client->peerAddress().toString()
                 << "in thread:" << client->thread();

        // ✅ 正确：使用Qt::QueuedConnection确保跨线程安全
        connect(client, &QTcpSocket::readyRead,
                this, [this, client]() {
                    QByteArray data = client->readAll();
                    emit clientDataReady(client, data);
                }, Qt::QueuedConnection);

        connect(client, &QTcpSocket::disconnected,
                this, &GoodNetworkServer::handleClientDisconnected, Qt::QueuedConnection);

        // 发射信号通知其他线程
        emit clientConnected(client);
    }
}

void GoodNetworkServer::handleClientDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        return;
    }

    qDebug() << "[DEBUG] Client disconnected:" << client->peerAddress().toString();

    m_clients.removeAll(client);
    m_clientBuffers.remove(client);
    client->deleteLater();
}

void GoodNetworkServer::processClientData() {
    qDebug() << "[DEBUG] Processing client data in thread:" << QThread::currentThread();

    // 这个槽可以在不同线程中安全地处理数据
}

// 线程管理的正确示例
class NetworkThread : public QThread {
    Q_OBJECT

public:
    explicit NetworkThread(QObject *parent = nullptr)
        : QThread(parent) {}

protected:
    void run() override {
        qDebug() << "[DEBUG] Network thread started:" << QThread::currentThread();

        // 在这个线程中创建网络服务器
        GoodNetworkServer *server = new GoodNetworkServer();

        // 将服务器移动到这个线程
        server->moveToThread(this);

        // 连接信号
        connect(this, &NetworkThread::startServerRequested, server, &GoodNetworkServer::startServer);

        // 启动事件循环
        exec();

        qDebug() << "[DEBUG] Network thread ended";
        server->deleteLater();
    }

signals:
    void startServerRequested();
};

// 线程启动同步的正确示例
class ThreadSynchronizationExample : public QObject {
    Q_OBJECT

public:
    explicit ThreadSynchronizationExample(QObject *parent = nullptr);

public slots:
    void startNetworkService();
    bool waitForNetworkCompletion(int timeoutMs = 5000);

signals:
    void networkStarted();
    void networkError(const QString &error);
    void componentsInitialized();

private slots:
    void onNetworkThreadStarted();
    void onNetworkThreadFinished();

private:
    NetworkThread *m_networkThread;
    bool m_isInitialized;
};

ThreadSynchronizationExample::ThreadSynchronizationExample(QObject *parent)
    : QObject(parent), m_networkThread(nullptr), m_isInitialized(false) {
}

void ThreadSynchronizationExample::startNetworkService() {
    qDebug() << "[DEBUG] Starting network service...";

    if (m_networkThread) {
        qWarning() << "Network service already running";
        return;
    }

    m_isInitialized = false;
    m_networkThread = new NetworkThread(this);

    // ✅ 正确：连接线程生命周期信号
    connect(m_networkThread, &QThread::started, this, &ThreadSynchronizationExample::onNetworkThreadStarted);
    connect(m_networkThread, &QThread::finished, this, &ThreadSynchronizationExample::onNetworkThreadFinished);

    // 启动线程
    m_networkThread->start();
}

void ThreadSynchronizationExample::onNetworkThreadStarted() {
    qDebug() << "[DEBUG] Network thread started successfully";

    // 发射启动服务器信号
    emit m_networkThread->startServerRequested();

    // 模拟组件初始化完成
    QTimer::singleShot(1000, [this]() {
        m_isInitialized = true;
        emit componentsInitialized();
        emit networkStarted();
    });
}

void ThreadSynchronizationExample::onNetworkThreadFinished() {
    qDebug() << "[DEBUG] Network thread finished";
    m_networkThread->deleteLater();
    m_networkThread = nullptr;
    m_isInitialized = false;
}

bool ThreadSynchronizationExample::waitForNetworkCompletion(int timeoutMs) {
    qDebug() << "[DEBUG] Waiting for network completion...";

    QEventLoop loop;
    QTimer timer;
    bool success = false;

    // ✅ 正确：等待实际的初始化信号，而不是客户端连接
    connect(this, &ThreadSynchronizationExample::networkError, &loop, &QEventLoop::quit);
    connect(this, &ThreadSynchronizationExample::componentsInitialized, &loop, &QEventLoop::quit);

    timer.setSingleShot(true);
    timer.setInterval(timeoutMs);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    timer.start();
    loop.exec();

    if (timer.isActive()) {
        success = true;
        qDebug() << "[DEBUG] Network components initialized successfully";
    } else {
        qCritical() << "[DEBUG] Network initialization timeout";
    }

    return success;
}
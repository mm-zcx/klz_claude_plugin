/*
 * Qt信号连接最佳实践示例
 * 基于WRJZH项目的实际经验和问题总结
 */

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <memory>

// ===== 数据结构定义 =====
struct StatusReply {
    int workMode = 0;
    int loadPosition = 0;
    bool workStatus = false;
};

struct SelfcheckReply {
    int channelState = 0;
    int freqConvState = 0;
    int mainCtrlState = 0;
    int powerState = 0;
    int timeState = 0;
};

// ===== 正确的发送者类实现 =====
class ServiceFacade : public QObject
{
    Q_OBJECT

public:
    explicit ServiceFacade(QObject *parent = nullptr)
        : QObject(parent)
        // ✅ 使用成员初始化列表初始化所有变量
        , m_workMode(false)           // 明确初始化
        , m_interferenceFlag(false)   // 明确初始化
        , m_reconModeFlag(false)      // 明确初始化
        , m_jamMode(0)                // 明确初始化
    {
        // ✅ 结构体使用值初始化
        m_status = StatusReply{};
        m_selfcheck = SelfcheckReply{};

        qDebug() << "ServiceFacade initialized successfully";
    }

signals:
    // ✅ 明确的信号定义，参数类型清晰
    void statusReplyReady(const StatusReply &status);
    void selfcheckReplyReady(const SelfcheckReply &selfcheck);

public slots:
    void handleWorkModeChange(int mode) {
        m_workMode = static_cast<bool>(mode);

        // ✅ 更新状态并立即发送信号
        m_status.workMode = mode;
        emit statusReplyReady(m_status);
        emit selfcheckReplyReady(m_selfcheck);
    }

private:
    // ✅ 所有成员变量都有明确的初始值
    bool m_workMode;
    bool m_interferenceFlag;
    bool m_reconModeFlag;
    int m_jamMode;

    StatusReply m_status;
    SelfcheckReply m_selfcheck;
};

// ===== 正确的接收者类实现 =====
class ProtocolParser : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolParser(QObject *parent = nullptr)
        : QObject(parent)
        // ✅ 成员初始化列表
        , m_dataCheckFlag(true)
    {
        // ✅ 结构体使用memset或值初始化
        memset(&m_status, 0, sizeof(m_status));
        memset(&m_selfcheck, 0, sizeof(m_selfcheck));

        qDebug() << "ProtocolParser initialized successfully";
    }

public slots:
    // ✅ 避免重载，使用明确的函数名
    void slot_UpdateStatus(const StatusReply &data) {
        qDebug() << "Status update received - workMode:" << data.workMode;

        // ✅ 直接赋值，避免未定义行为
        m_status.loadPosition = data.loadPosition;
        m_status.workMode = data.workMode;
        m_status.workStatus = data.workStatus;
    }

    void slot_UpdateSelfcheck(const SelfcheckReply &data) {
        qDebug() << "Selfcheck update received - channelState:" << data.channelState;

        m_selfcheck.channelState = data.channelState;
        m_selfcheck.freqConvState = data.freqConvState;
        m_selfcheck.mainCtrlState = data.mainCtrlState;
        m_selfcheck.powerState = data.powerState;
        m_selfcheck.timeState = data.timeState;
    }

private:
    bool m_dataCheckFlag;
    StatusReply m_status;
    SelfcheckReply m_selfcheck;
};

// ===== 应用程序类 - 正确的连接方式 =====
class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject *parent = nullptr)
        : QObject(parent)
        , m_connectionRetryCount(0)
    {
        initializeComponents();
    }

    void initializeComponents() {
        // ✅ 创建组件
        m_serviceFacade = std::make_unique<ServiceFacade>();
        m_protocolParser = std::make_unique<ProtocolParser>();

        // ✅ 立即尝试连接信号
        setupProtocolDataConnection();
    }

private slots:
    void setupProtocolDataConnection() {
        if (m_serviceFacade && m_protocolParser) {
            // ✅ 使用明确的槽函数名，避免重载歧义
            bool statusConnected = connect(
                m_serviceFacade.get(), &ServiceFacade::statusReplyReady,
                m_protocolParser.get(), &ProtocolParser::slot_UpdateStatus
            );

            bool selfcheckConnected = connect(
                m_serviceFacade.get(), &ServiceFacade::selfcheckReplyReady,
                m_protocolParser.get(), &ProtocolParser::slot_UpdateSelfcheck
            );

            // ✅ 验证连接状态
            if (statusConnected && selfcheckConnected) {
                qDebug() << "✅ Protocol data connection established successfully";
                m_connectionRetryCount = 0;
            } else {
                qWarning() << "❌ Failed to establish protocol data connection";
                qDebug() << "  Status connected:" << statusConnected;
                qDebug() << "  Selfcheck connected:" << selfcheckConnected;
            }
        } else {
            // ✅ 重试机制处理初始化时机问题
            if (m_connectionRetryCount < MAX_CONNECTION_RETRIES) {
                m_connectionRetryCount++;
                qWarning() << "⏳ Components not ready, retrying in 1 second... ("
                         << m_connectionRetryCount << "/" << MAX_CONNECTION_RETRIES << ")";

                QTimer::singleShot(1000, this, &Application::setupProtocolDataConnection);
            } else {
                qCritical() << "❌ Failed to establish connection after maximum retries";
            }
        }
    }

    void testSignalConnection() {
        if (m_serviceFacade) {
            qDebug() << "🧪 Testing signal connection...";
            m_serviceFacade->handleWorkModeChange(3);  // 发送测试信号
        }
    }

private:
    std::unique_ptr<ServiceFacade> m_serviceFacade;
    std::unique_ptr<ProtocolParser> m_protocolParser;

    int m_connectionRetryCount;
    static const int MAX_CONNECTION_RETRIES = 10;
};

// ===== 多线程连接示例 =====
class ThreadedConnection : public QObject
{
    Q_OBJECT

public:
    ThreadedConnection() {
        // 创建不同线程的对象
        m_sender = new ServiceFacade();
        m_receiver = new ProtocolParser();

        // 将接收者移动到不同线程
        m_receiverThread = new QThread();
        m_receiver->moveToThread(m_receiverThread);

        // ✅ 跨线程连接使用Qt::QueuedConnection
        connect(m_sender, &ServiceFacade::statusReplyReady,
                m_receiver, &ProtocolParser::slot_UpdateStatus,
                Qt::QueuedConnection);

        connect(m_sender, &ServiceFacade::selfcheckReplyReady,
                m_receiver, &ProtocolParser::slot_UpdateSelfcheck,
                Qt::QueuedConnection);

        m_receiverThread->start();

        qDebug() << "🧵 Cross-thread connection established with Qt::QueuedConnection";
    }

    ~ThreadedConnection() {
        m_receiverThread->quit();
        m_receiverThread->wait();
        delete m_receiverThread;
        delete m_sender;
        delete m_receiver;
    }

private:
    ServiceFacade* m_sender;
    ProtocolParser* m_receiver;
    QThread* m_receiverThread;
};

// ===== 错误示例对比 =====
class BadExample : public QObject
{
    Q_OBJECT

public:
    BadExample(QObject *parent = nullptr) : QObject(parent) {
        // ❌ 错误1：成员变量未初始化
        // m_flag 未初始化，包含随机值

        // ❌ 错误2：结构体未初始化
        // m_status 成员包含随机内存内容
    }

signals:
    void someSignal(int value);

public slots:
    // ❌ 错误3：槽函数重载歧义
    void slot_HandleData(int data) { /* 处理逻辑 */ }
    void slot_HandleData(QString data) { /* 处理逻辑 */ }

    void connectSignals() {
        BadExample* receiver = new BadExample();

        // ❌ 错误4：编译错误 - 重载歧义
        // connect(this, &BadExample::someSignal, receiver, &BadExample::slot_HandleData);
        // 错误信息: C2665: "QObject::connect": 没有重载函数可以转换所有参数类型

        // ❌ 错误5：未检查连接状态
        connect(this, &BadExample::someSignal, receiver,
                static_cast<void(BadExample::*)(int)>(&BadExample::slot_HandleData));
        // 没有验证连接是否成功

        delete receiver;  // ❌ 错误6：对象生命周期管理问题
    }

private:
    bool m_flag;  // ❌ 未初始化
    StatusReply m_status;  // ❌ 未初始化
};

// ===== 调试和验证宏 =====
#ifdef QT_DEBUG
#define VERIFY_CONNECTION(sender, signal, receiver, slot) \
    do { \
        bool connected = connect(sender, signal, receiver, slot); \
        Q_ASSERT_X(connected, "VERIFY_CONNECTION", "Signal connection failed"); \
        qDebug() << "✅ Connected:" << #sender << "->" << #receiver; \
    } while(0)

#define CHECK_INITIALIZED(member, expected_value) \
    Q_ASSERT_X(member == (expected_value), #member, "Member not properly initialized")
#else
#define VERIFY_CONNECTION(sender, signal, receiver, slot) \
    connect(sender, signal, receiver, slot)

#define CHECK_INITIALIZED(member, expected_value)
#endif

// ===== 最佳实践总结 =====
/*
 ✅ 最佳实践:
 1. 使用成员初始化列表初始化所有变量
 2. 避免槽函数重载，使用明确的函数名
 3. 验证信号连接状态
 4. 实现重试机制处理初始化时机问题
 5. 跨线程连接使用Qt::QueuedConnection
 6. 管理对象生命周期
 7. 使用调试宏验证连接和初始化

 ❌ 常见错误:
 1. 成员变量未初始化
 2. 槽函数重载导致歧义
 3. 不检查连接状态
 4. 跨线程连接缺少QueuedConnection
 5. 对象生命周期管理不当
 6. 结构体未正确初始化
 */

#include "signal_connection_best_practices.moc"
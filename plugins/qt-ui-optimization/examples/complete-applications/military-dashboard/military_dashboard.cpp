#include "military_dashboard.h"
#include <QDateTime>
#include <QMessageBox>
#include <QSplitter>

MilitaryDashboard::MilitaryDashboard(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_tabWidget(nullptr)
    , m_radarAngle(0)
    , m_isScanning(false)
{
    setWindowTitle("军工仪表盘 - 战术监控系统");
    setMinimumSize(1200, 800);

    setupUI();
    setupMenuBar();
    setupStatusBar();

    // 初始化定时器
    m_radarTimer = new QTimer(this);
    connect(m_radarTimer, &QTimer::timeout, this, &MilitaryDashboard::updateRadarData);

    m_statusTimer = new QTimer(this);
    connect(m_statusTimer, &QTimer::timeout, this, &MilitaryDashboard::updateSystemStatus);
    m_statusTimer->start(1000);

    // 初始状态更新
    updateSystemStatus();
}

MilitaryDashboard::~MilitaryDashboard()
{
}

void MilitaryDashboard::setupUI()
{
    setupCentralWidget();
}

void MilitaryDashboard::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    // 系统菜单
    QMenu *systemMenu = menuBar->addMenu("系统");
    QAction *systemInfoAction = systemMenu->addAction("系统信息");
    connect(systemInfoAction, &QAction::triggered, this, &MilitaryDashboard::showSystemInfo);
    systemMenu->addSeparator();
    QAction *exitAction = systemMenu->addAction("退出");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 视图菜单
    QMenu *viewMenu = menuBar->addMenu("视图");
    QAction *radarAction = viewMenu->addAction("雷达视图");
    QAction *controlAction = viewMenu->addAction("控制面板");
    QAction *statusAction = viewMenu->addAction("状态监控");

    // 工具菜单
    QMenu *toolsMenu = menuBar->addMenu("工具");
    QAction *scanAction = toolsMenu->addAction("系统扫描");
    connect(scanAction, &QAction::triggered, this, &MilitaryDashboard::onSystemScan);

    // 帮助菜单
    QMenu *helpMenu = menuBar->addMenu("帮助");
    QAction *aboutAction = helpMenu->addAction("关于");
}

void MilitaryDashboard::setupStatusBar()
{
    QStatusBar *statusBar = this->statusBar();

    // 状态标签
    m_statusLabel = new QLabel("系统就绪");
    statusBar->addWidget(m_statusLabel);

    // 连接状态
    m_connectionLabel = new QLabel("连接状态: 在线");
    m_connectionLabel->setStyleSheet("color: #2ECC71;");
    statusBar->addPermanentWidget(m_connectionLabel);

    // 时间显示
    m_timeLabel = new QLabel();
    statusBar->addPermanentWidget(m_timeLabel);
}

void MilitaryDashboard::setupCentralWidget()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    // 创建分割器
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, m_centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->addWidget(mainSplitter);

    // 左侧面板
    QWidget *leftPanel = new QWidget();
    leftPanel->setMaximumWidth(400);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    setupRadarPanel();
    setupControlPanel();

    leftLayout->addWidget(m_radarGroup);
    leftLayout->addWidget(m_controlGroup);
    leftLayout->addStretch();

    // 右侧面板
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    setupStatusPanel();
    setupDataPanel();

    rightLayout->addWidget(m_statusGroup);
    rightLayout->addWidget(m_dataGroup);

    // 添加到分割器
    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 2);
}

void MilitaryDashboard::setupRadarPanel()
{
    m_radarGroup = new QGroupBox("雷达监控系统");
    m_radarGroup->setObjectName("radar-display");

    QVBoxLayout *layout = new QVBoxLayout(m_radarGroup);

    // 雷达显示区域
    m_radarDisplay = new QWidget();
    m_radarDisplay->setMinimumHeight(200);
    m_radarDisplay->setStyleSheet(R"(
        QWidget {
            background: radial-gradient(circle,
                rgba(46, 204, 113, 0.1) 0%,
                rgba(26, 26, 26, 0.8) 50%,
                rgba(26, 26, 26, 0.95) 100%);
            border: 2px solid #2ECC71;
            border-radius: 50%;
        }
    })");

    QHBoxLayout *radarLayout = new QHBoxLayout(m_radarDisplay);
    m_radarStatus = new QLabel("雷达就绪\n按扫描开始");
    m_radarStatus->setAlignment(Qt::AlignCenter);
    m_radarStatus->setStyleSheet("color: #2ECC71; font-size: 14px; font-weight: bold;");
    radarLayout->addWidget(m_radarStatus);

    // 控制按钮
    m_scanButton = new QPushButton("开始扫描");
    m_scanButton->setProperty("class", "tactical");
    connect(m_scanButton, &QPushButton::clicked, this, [this]() {
        if (m_isScanning) {
            m_isScanning = false;
            m_radarTimer->stop();
            m_scanButton->setText("开始扫描");
            m_radarStatus->setText("雷达就绪\n按扫描开始");
        } else {
            m_isScanning = true;
            m_radarTimer->start(100);
            m_scanButton->setText("停止扫描");
            m_radarStatus->setText("扫描中...\n角度: 0°");
        }
    });

    layout->addWidget(m_radarDisplay);
    layout->addWidget(m_scanButton);
}

void MilitaryDashboard::setupControlPanel()
{
    m_controlGroup = new QGroupBox("战术控制面板");
    QVBoxLayout *layout = new QVBoxLayout(m_controlGroup);

    // 战术行动按钮
    m_tacticalButton = new QPushButton("战术行动");
    m_tacticalButton->setProperty("class", "tactical");
    m_tacticalButton->setIconText("🎯");
    connect(m_tacticalButton, &QPushButton::clicked, this, &MilitaryDashboard::onTacticalAction);

    // 紧急停止按钮
    m_emergencyButton = new QPushButton("紧急停止");
    m_emergencyButton->setProperty("class", "hud");
    m_emergencyButton->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E74C3C, stop:1 #C0392B);");
    connect(m_emergencyButton, &QPushButton::clicked, this, &MilitaryDashboard::onEmergencyStop);

    // 系统按钮
    m_systemButton = new QPushButton("系统检查");
    m_systemButton->setProperty("class", "hud");
    connect(m_systemButton, &QPushButton::clicked, this, &MilitaryDashboard::onSystemScan);

    // 功率控制
    QLabel *powerLabel = new QLabel("功率控制:");
    m_powerSlider = new QSlider(Qt::Horizontal);
    m_powerSlider->setRange(0, 100);
    m_powerSlider->setValue(75);
    m_powerLabel = new QLabel("75%");
    connect(m_powerSlider, &QSlider::valueChanged, this, [this](int value) {
        m_powerLabel->setText(QString("%1%").arg(value));
    });

    // 模式选择
    QLabel *modeLabel = new QLabel("运行模式:");
    m_modeCombo = new QComboBox();
    m_modeCombo->addItems({"待机模式", "巡逻模式", "战斗模式", "紧急模式"});

    // 布局
    layout->addWidget(m_tacticalButton);
    layout->addWidget(m_emergencyButton);
    layout->addWidget(m_systemButton);
    layout->addSpacing(20);
    layout->addWidget(powerLabel);
    layout->addWidget(m_powerSlider);
    layout->addWidget(m_powerLabel);
    layout->addSpacing(10);
    layout->addWidget(modeLabel);
    layout->addWidget(m_modeCombo);
}

void MilitaryDashboard::setupStatusPanel()
{
    m_statusGroup = new QGroupBox("系统状态监控");
    m_statusGroup->setObjectName("status-panel");
    QVBoxLayout *layout = new QVBoxLayout(m_statusGroup);

    // 系统状态
    m_systemLabel = new QLabel("系统状态:");
    m_systemProgress = new QProgressBar();
    m_systemProgress->setRange(0, 100);
    m_systemProgress->setValue(85);

    // 通信状态
    m_communicationLabel = new QLabel("通信状态:");
    m_communicationProgress = new QProgressBar();
    m_communicationProgress->setRange(0, 100);
    m_communicationProgress->setValue(92);

    // 武器状态
    m_weaponLabel = new QLabel("武器系统:");
    m_weaponProgress = new QProgressBar();
    m_weaponProgress->setRange(0, 100);
    m_weaponProgress->setValue(78);

    layout->addWidget(m_systemLabel);
    layout->addWidget(m_systemProgress);
    layout->addWidget(m_communicationLabel);
    layout->addWidget(m_communicationProgress);
    layout->addWidget(m_weaponLabel);
    layout->addWidget(m_weaponProgress);
}

void MilitaryDashboard::setupDataPanel()
{
    m_dataGroup = new QGroupBox("数据分析");
    QVBoxLayout *layout = new QVBoxLayout(m_dataGroup);

    // 数据表格
    m_dataTable = new QTableWidget(5, 3);
    m_dataTable->setHorizontalHeaderLabels({"参数", "当前值", "状态"});
    m_dataTable->verticalHeader()->setVisible(false);

    // 填充示例数据
    m_dataTable->setItem(0, 0, new QTableWidgetItem("温度"));
    m_dataTable->setItem(0, 1, new QTableWidgetItem("23.5°C"));
    m_dataTable->setItem(0, 2, new QTableWidgetItem("正常"));

    m_dataTable->setItem(1, 0, new QTableWidgetItem("压力"));
    m_dataTable->setItem(1, 1, new QTableWidgetItem("101.3 kPa"));
    m_dataTable->setItem(1, 2, new QTableWidgetItem("正常"));

    m_dataTable->setItem(2, 0, new QTableWidgetItem("湿度"));
    m_dataTable->setItem(2, 1, new QTableWidgetItem("45%"));
    m_dataTable->setItem(2, 2, new QTableWidgetItem("正常"));

    m_dataTable->setItem(3, 0, new QTableWidgetItem("电压"));
    m_dataTable->setItem(3, 1, new QTableWidgetItem("12.6V"));
    m_dataTable->setItem(3, 2, new QTableWidgetItem("良好"));

    m_dataTable->setItem(4, 0, new QTableWidgetItem("信号强度"));
    m_dataTable->setItem(4, 1, new QTableWidgetItem("-45 dBm"));
    m_dataTable->setItem(4, 2, new QTableWidgetItem("优秀"));

    // 调整表格
    m_dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_dataTable->setAlternatingRowColors(true);

    // 日志显示
    QLabel *logLabel = new QLabel("系统日志:");
    m_logDisplay = new QTextEdit();
    m_logDisplay->setMaximumHeight(150);
    m_logDisplay->setPlainText("[2024-01-01 10:00:00] 系统启动完成\n[2024-01-01 10:00:01] 自检通过\n[2024-01-01 10:00:02] 连接网络成功");

    layout->addWidget(m_dataTable);
    layout->addWidget(logLabel);
    layout->addWidget(m_logDisplay);
}

void MilitaryDashboard::updateRadarData()
{
    m_radarAngle = (m_radarAngle + 6) % 360;
    m_radarStatus->setText(QString("扫描中...\n角度: %1°").arg(m_radarAngle));
}

void MilitaryDashboard::updateSystemStatus()
{
    // 更新时间
    QDateTime currentTime = QDateTime::currentDateTime();
    m_timeLabel->setText(currentTime.toString("yyyy-MM-dd hh:mm:ss"));

    // 模拟状态变化
    static int counter = 0;
    counter++;

    if (counter % 3 == 0) {
        int systemValue = 80 + (qrand() % 15);
        m_systemProgress->setValue(systemValue);

        int commValue = 85 + (qrand() % 10);
        m_communicationProgress->setValue(commValue);

        int weaponValue = 70 + (qrand() % 20);
        m_weaponProgress->setValue(weaponValue);
    }

    // 更新状态标签
    QStringList statuses = {"系统就绪", "监控中", "数据更新", "扫描完成"};
    m_statusLabel->setText(statuses[counter % statuses.size()]);
}

void MilitaryDashboard::onTacticalAction()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "战术行动确认",
        "确定要执行战术行动吗？\n此操作将激活相关系统。",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_statusLabel->setText("战术行动执行中...");
        m_logDisplay->append(QString("[%1] 战术行动已激活")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }
}

void MilitaryDashboard::onSystemScan()
{
    m_statusLabel->setText("系统扫描中...");

    // 模拟扫描过程
    for (int i = 0; i <= 100; i += 10) {
        m_systemProgress->setValue(i);
        QApplication::processEvents();
    }

    m_statusLabel->setText("系统扫描完成");
    m_logDisplay->append(QString("[%1] 系统扫描完成，所有系统正常")
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
}

void MilitaryDashboard::onEmergencyStop()
{
    QMessageBox::warning(
        this,
        "紧急停止",
        "紧急停止已激活！\n所有系统将进入安全模式。"
    );

    m_statusLabel->setText("紧急停止状态");
    m_isScanning = false;
    m_radarTimer->stop();
    m_scanButton->setText("开始扫描");
    m_radarStatus->setText("系统已停止\n等待重启");

    m_logDisplay->append(QString("[%1] 紧急停止已执行")
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
}

void MilitaryDashboard::showSystemInfo()
{
    QString info = R"(
军工仪表盘 v1.0

系统信息:
- 操作系统: Windows/Linux
- Qt版本: 6.x
- 编译时间: %1
- 系统状态: 正常运行

功能特性:
- 实时雷达监控
- 战术控制面板
- 系统状态监控
- 数据分析功能
- 紧急响应系统

技术支持:
- 24/7 技术支持热线
- 远程诊断服务
- 定期系统更新
)";

    QMessageBox::information(
        this,
        "系统信息",
        info.arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
    );
}
#include "dashboard.h"
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QRandomGenerator>

Dashboard::Dashboard(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_headerWidget(nullptr)
    , m_headerLayout(nullptr)
    , m_titleLabel(nullptr)
    , m_subtitleLabel(nullptr)
    , m_refreshButton(nullptr)
    , m_exportButton(nullptr)
    , m_settingsButton(nullptr)
    , m_themeCombo(nullptr)
    , m_sidebarWidget(nullptr)
    , m_sidebarLayout(nullptr)
    , m_statisticsGroup(nullptr)
    , m_totalUsersLabel(nullptr)
    , m_activeProjectsLabel(nullptr)
    , m_completionRateLabel(nullptr)
    , m_projectProgress(nullptr)
    , m_contentSplitter(nullptr)
    , m_tabWidget(nullptr)
    , m_tableTab(nullptr)
    , m_dataTable(nullptr)
    , m_chartTab(nullptr)
    , m_chartContainer(nullptr)
    , m_controlTab(nullptr)
    , m_controlScrollArea(nullptr)
    , m_controlWidget(nullptr)
    , m_updateTimer(nullptr)
{
    setupUI();
    connectSignals();
    loadSampleData();

    // 启动定时更新
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &Dashboard::updateStatistics);
    m_updateTimer->start(5000); // 每5秒更新一次
}

Dashboard::~Dashboard()
{
}

void Dashboard::setupUI()
{
    // 设置窗口属性
    setWindowTitle("Qt UI优化示例 - 现代仪表盘");
    setMinimumSize(1200, 800);
    resize(1400, 900);

    // 创建中央部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    // 创建主布局
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // 设置各个区域
    setupHeader();
    setupSidebar();
    setupMainContent();
    setupStatusBar();

    // 创建主分割器
    m_contentSplitter = new QSplitter(Qt::Horizontal, m_centralWidget);
    m_contentSplitter->addWidget(m_sidebarWidget);
    m_contentSplitter->addWidget(m_tabWidget);
    m_contentSplitter->setStretchFactor(0, 1);
    m_contentSplitter->setStretchFactor(1, 3);
    m_contentSplitter->setStyleSheet("QSplitter::handle { background: #E0E0E0; width: 1px; }");

    m_mainLayout->addWidget(m_contentSplitter);
}

void Dashboard::setupHeader()
{
    m_headerWidget = new QWidget();
    m_headerWidget->setFixedHeight(80);
    m_headerWidget->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #F5F7FA, stop:1 #FFFFFF); border-bottom: 1px solid #E0E0E0;");

    m_headerLayout = new QHBoxLayout(m_headerWidget);
    m_headerLayout->setContentsMargins(24, 16, 24, 16);
    m_headerLayout->setSpacing(16);

    // 标题区域
    QWidget *titleWidget = new QWidget();
    QVBoxLayout *titleLayout = new QVBoxLayout(titleWidget);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(4);

    m_titleLabel = new QLabel("数据仪表盘");
    m_titleLabel->setStyleSheet("font-size: 24px; font-weight: 600; color: #1976D2;");
    m_subtitleLabel = new QLabel("实时数据监控与分析平台");
    m_subtitleLabel->setStyleSheet("font-size: 14px; color: #616161;");

    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_subtitleLabel);

    // 控制按钮区域
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(12);

    // 主题选择器
    m_themeCombo = new QComboBox();
    m_themeCombo->addItem("现代蓝色", "modern-blue");
    m_themeCombo->addItem("深色主题", "dark-theme");
    m_themeCombo->addItem("浅色主题", "light-theme");
    m_themeCombo->setFixedWidth(120);
    m_themeCombo->setStyleSheet("QComboBox { padding: 6px 12px; }");

    // 功能按钮
    m_refreshButton = new QPushButton("刷新数据");
    applyButtonStyle(m_refreshButton, "primary");

    m_exportButton = new QPushButton("导出报告");
    applyButtonStyle(m_exportButton, "secondary");

    m_settingsButton = new QPushButton("⚙️");
    m_settingsButton->setFixedSize(36, 36);
    m_settingsButton->setStyleSheet("QPushButton { background: #F5F7FA; border: 1px solid #E0E0E0; border-radius: 18px; font-size: 16px; } QPushButton:hover { background: #E3F2FD; border-color: #2196F3; }");

    controlLayout->addWidget(new QLabel("主题:"));
    controlLayout->addWidget(m_themeCombo);
    controlLayout->addWidget(m_refreshButton);
    controlLayout->addWidget(m_exportButton);
    controlLayout->addWidget(m_settingsButton);

    m_headerLayout->addWidget(titleWidget);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(controlWidget);

    m_mainLayout->addWidget(m_headerWidget);
    m_mainLayout->setDirection(QBoxLayout::TopToBottom);
}

void Dashboard::setupSidebar()
{
    m_sidebarWidget = new QWidget();
    m_sidebarWidget->setFixedWidth(300);
    m_sidebarWidget->setStyleSheet("background: #FAFAFA; border-right: 1px solid #E0E0E0;");

    m_sidebarLayout = new QVBoxLayout(m_sidebarWidget);
    m_sidebarLayout->setContentsMargins(16, 16, 16, 16);
    m_sidebarLayout->setSpacing(16);

    // 统计信息组
    m_statisticsGroup = new QGroupBox("系统统计");
    m_statisticsGroup->setStyleSheet("QGroupBox { font-weight: 600; color: #1976D2; }");
    QVBoxLayout *statsLayout = new QVBoxLayout(m_statisticsGroup);

    // 用户总数
    QWidget *usersWidget = new QWidget();
    QHBoxLayout *usersLayout = new QHBoxLayout(usersWidget);
    usersLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *usersIcon = new QLabel("👥");
    usersIcon->setStyleSheet("font-size: 24px;");
    QVBoxLayout *usersInfoLayout = new QVBoxLayout();
    usersInfoLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *usersTitle = new QLabel("用户总数");
    usersTitle->setStyleSheet("font-size: 12px; color: #616161;");
    m_totalUsersLabel = new QLabel("1,234");
    m_totalUsersLabel->setStyleSheet("font-size: 20px; font-weight: 600; color: #37474F;");

    usersInfoLayout->addWidget(usersTitle);
    usersInfoLayout->addWidget(m_totalUsersLabel);
    usersLayout->addWidget(usersIcon);
    usersLayout->addLayout(usersInfoLayout);
    usersLayout->addStretch();

    // 活跃项目
    QWidget *projectsWidget = new QWidget();
    QHBoxLayout *projectsLayout = new QHBoxLayout(projectsWidget);
    projectsLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *projectsIcon = new QLabel("📁");
    projectsIcon->setStyleSheet("font-size: 24px;");
    QVBoxLayout *projectsInfoLayout = new QVBoxLayout();
    projectsInfoLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *projectsTitle = new QLabel("活跃项目");
    projectsTitle->setStyleSheet("font-size: 12px; color: #616161;");
    m_activeProjectsLabel = new QLabel("56");
    m_activeProjectsLabel->setStyleSheet("font-size: 20px; font-weight: 600; color: #37474F;");

    projectsInfoLayout->addWidget(projectsTitle);
    projectsInfoLayout->addWidget(m_activeProjectsLabel);
    projectsLayout->addWidget(projectsIcon);
    projectsLayout->addLayout(projectsInfoLayout);
    projectsLayout->addStretch();

    // 完成率
    QWidget *completionWidget = new QWidget();
    QVBoxLayout *completionLayout = new QVBoxLayout(completionWidget);
    completionLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *completionTitle = new QLabel("项目完成率");
    completionTitle->setStyleSheet("font-size: 12px; color: #616161;");
    m_completionRateLabel = new QLabel("78%");
    m_completionRateLabel->setStyleSheet("font-size: 20px; font-weight: 600; color: #4CAF50;");

    m_projectProgress = new QProgressBar();
    createModernProgressBar(m_projectProgress, 78);

    completionLayout->addWidget(completionTitle);
    completionLayout->addWidget(m_completionRateLabel);
    completionLayout->addWidget(m_projectProgress);

    statsLayout->addWidget(usersWidget);
    statsLayout->addWidget(projectsWidget);
    statsLayout->addWidget(completionWidget);

    // 快速操作
    QGroupBox *quickActionsGroup = new QGroupBox("快速操作");
    quickActionsGroup->setStyleSheet("QGroupBox { font-weight: 600; color: #1976D2; }");
    QVBoxLayout *quickLayout = new QVBoxLayout(quickActionsGroup);

    QStringList actions = {"新建项目", "导入数据", "生成报告", "系统设置"};
    for (const QString &action : actions) {
        QPushButton *actionBtn = new QPushButton(action);
        applyButtonStyle(actionBtn, "secondary");
        quickLayout->addWidget(actionBtn);
    }

    m_sidebarLayout->addWidget(m_statisticsGroup);
    m_sidebarLayout->addWidget(quickActionsGroup);
    m_sidebarLayout->addStretch();
}

void Dashboard::setupMainContent()
{
    m_tabWidget = new QTabWidget();
    m_tabWidget->setStyleSheet("QTabBar::tab { padding: 12px 24px; }");

    // 数据表格标签页
    m_tableTab = new QWidget();
    QVBoxLayout *tableLayout = new QVBoxLayout(m_tableTab);
    tableLayout->setContentsMargins(16, 16, 16, 16);

    // 表格工具栏
    QWidget *tableToolbar = new QWidget();
    QHBoxLayout *toolbarLayout = new QHBoxLayout(tableToolbar);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);

    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("搜索数据...");
    searchEdit->setMaximumWidth(300);

    QPushButton *filterBtn = new QPushButton("筛选");
    QPushButton *addBtn = new QPushButton("添加");
    applyButtonStyle(filterBtn, "secondary");
    applyButtonStyle(addBtn, "primary");

    toolbarLayout->addWidget(new QLabel("数据列表"));
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(searchEdit);
    toolbarLayout->addWidget(filterBtn);
    toolbarLayout->addWidget(addBtn);

    // 数据表格
    m_dataTable = new QTableWidget(10, 6);
    QStringList headers = {"ID", "项目名称", "负责人", "状态", "进度", "创建时间"};
    m_dataTable->setHorizontalHeaderLabels(headers);

    // 设置表格样式
    m_dataTable->horizontalHeader()->setStretchLastSection(true);
    m_dataTable->horizontalHeader()->setDefaultSectionSize(120);
    m_dataTable->setAlternatingRowColors(true);
    m_dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableLayout->addWidget(tableToolbar);
    tableLayout->addWidget(m_dataTable);

    // 图表标签页
    m_chartTab = new QWidget();
    QVBoxLayout *chartLayout = new QVBoxLayout(m_chartTab);
    chartLayout->setContentsMargins(16, 16, 16, 16);

    QLabel *chartTitle = new QLabel("数据分析图表");
    chartTitle->setStyleSheet("font-size: 18px; font-weight: 600; color: #37474F;");

    m_chartContainer = new QWidget();
    m_chartContainer->setMinimumHeight(400);
    m_chartContainer->setStyleSheet("background: #FFFFFF; border: 1px solid #E0E0E0; border-radius: 8px;");
    QVBoxLayout *chartContainerLayout = new QVBoxLayout(m_chartContainer);
    chartContainerLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *chartPlaceholder = new QLabel("📊 图表区域\n\n这里将显示数据可视化图表\n(需要集成图表库如Qt Charts)");
    chartPlaceholder->setAlignment(Qt::AlignCenter);
    chartPlaceholder->setStyleSheet("color: #616161; font-size: 16px;");
    chartContainerLayout->addWidget(chartPlaceholder);

    chartLayout->addWidget(chartTitle);
    chartLayout->addWidget(m_chartContainer);

    // 控制面板标签页
    m_controlTab = new QWidget();
    QVBoxLayout *controlLayout = new QVBoxLayout(m_controlTab);
    controlLayout->setContentsMargins(16, 16, 16, 16);

    m_controlScrollArea = new QScrollArea();
    m_controlScrollArea->setWidgetResizable(true);
    m_controlScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_controlWidget = new QWidget();
    QVBoxLayout *controlFormLayout = new QVBoxLayout(m_controlWidget);
    controlFormLayout->setContentsMargins(20, 20, 20, 20);
    controlFormLayout->setSpacing(20);

    // 控制表单
    QGroupBox *generalGroup = new QGroupBox("常规设置");
    QVBoxLayout *generalLayout = new QVBoxLayout(generalGroup);

    generalLayout->addWidget(new QLabel("应用名称:"));
    QLineEdit *appNameEdit = new QLineEdit("数据仪表盘");
    generalLayout->addWidget(appNameEdit);

    generalLayout->addWidget(new QLabel("数据刷新间隔(秒):"));
    QSpinBox *intervalSpin = new QSpinBox();
    intervalSpin->setRange(1, 3600);
    intervalSpin->setValue(5);
    generalLayout->addWidget(intervalSpin);

    generalLayout->addWidget(new QLabel("启用自动刷新:"));
    QCheckBox *autoRefreshCheck = new QCheckBox("启用自动数据刷新");
    autoRefreshCheck->setChecked(true);
    generalLayout->addWidget(autoRefreshCheck);

    QGroupBox *displayGroup = new QGroupBox("显示设置");
    QVBoxLayout *displayLayout = new QVBoxLayout(displayGroup);

    displayLayout->addWidget(new QLabel("每页显示行数:"));
    QComboBox *pageSizeCombo = new QComboBox();
    pageSizeCombo->addItems({"10", "25", "50", "100"});
    pageSizeCombo->setCurrentText("10");
    displayLayout->addWidget(pageSizeCombo);

    displayLayout->addWidget(new QLabel("主题选择:"));
    QComboBox *themeCombo = new QComboBox();
    themeCombo->addItems({"现代风格", "经典风格", "紧凑风格"});
    displayLayout->addWidget(themeCombo);

    controlFormLayout->addWidget(generalGroup);
    controlFormLayout->addWidget(displayGroup);
    controlFormLayout->addStretch();

    m_controlScrollArea->setWidget(m_controlWidget);
    controlLayout->addWidget(m_controlScrollArea);

    // 添加标签页
    m_tabWidget->addTab(m_tableTab, "📊 数据列表");
    m_tabWidget->addTab(m_chartTab, "📈 数据分析");
    m_tabWidget->addTab(m_controlTab, "⚙️ 控制面板");
}

void Dashboard::setupStatusBar()
{
    QStatusBar *statusBar = this->statusBar();
    statusBar->setStyleSheet("background: #F5F7FA; border-top: 1px solid #E0E0E0; color: #616161;");

    QLabel *statusLabel = new QLabel("就绪");
    QLabel *timeLabel = new QLabel();
    QLabel *versionLabel = new QLabel("v1.0.0");

    statusBar->addWidget(statusLabel);
    statusBar->addPermanentWidget(timeLabel);
    statusBar->addPermanentWidget(versionLabel);

    // 更新时间
    QTimer::timerEvent(new QTimerEvent(1000));
    connect(new QTimer(this), &QTimer::timeout, [timeLabel]() {
        timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });
}

void Dashboard::connectSignals()
{
    connect(m_refreshButton, &QPushButton::clicked, this, &Dashboard::onRefreshData);
    connect(m_exportButton, &QPushButton::clicked, this, &Dashboard::onExportReport);
    connect(m_settingsButton, &QPushButton::clicked, this, &Dashboard::onSettingsClicked);
    connect(m_themeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &Dashboard::onThemeChanged);
}

void Dashboard::loadSampleData()
{
    // 加载表格数据
    QStringList projects = {
        "电商平台开发", "移动APP重构", "数据分析系统", "客户管理系统",
        "在线教育平台", "金融交易系统", "物联网监控", "内容管理系统",
        "人工智能助手", "区块链钱包", "云计算平台", "社交网络应用"
    };

    QStringList managers = {
        "张三", "李四", "王五", "赵六", "钱七", "孙八",
        "周九", "吴十", "郑十一", "冯十二", "陈十三", "褚十四"
    };

    QStringList statuses = {"进行中", "已完成", "暂停", "计划中"};

    m_dataTable->setRowCount(projects.size());

    for (int i = 0; i < projects.size(); ++i) {
        m_dataTable->setItem(i, 0, new QTableWidgetItem(QString::number(1001 + i)));
        m_dataTable->setItem(i, 1, new QTableWidgetItem(projects[i]));
        m_dataTable->setItem(i, 2, new QTableWidgetItem(managers[i]));
        m_dataTable->setItem(i, 3, new QTableWidgetItem(statuses[i % statuses.size()]));

        int progress = QRandomGenerator::global()->bounded(20, 100);
        m_dataTable->setItem(i, 4, new QTableWidgetItem(QString("%1%").arg(progress)));

        QDateTime date = QDateTime::currentDateTime().addDays(-QRandomGenerator::global()->bounded(1, 365));
        m_dataTable->setItem(i, 5, new QTableWidgetItem(date.toString("yyyy-MM-dd")));
    }
}

void Dashboard::applyCardStyle(QWidget *widget)
{
    widget->setStyleSheet(
        "QWidget {"
        "   background: white;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 8px;"
        "   padding: 16px;"
        "}"
        "QWidget:hover {"
        "   border-color: #2196F3;"
        "   box-shadow: 0 4px 8px rgba(33, 150, 243, 0.1);"
        "}"
    );
}

void Dashboard::applyButtonStyle(QPushButton *button, const QString &styleClass)
{
    if (styleClass == "primary") {
        button->setProperty("class", "primary");
    } else if (styleClass == "secondary") {
        button->setProperty("class", "secondary");
    }
}

void Dashboard::createModernProgressBar(QProgressBar *progressBar, int value)
{
    progressBar->setValue(value);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: none;"
        "   border-radius: 4px;"
        "   text-align: center;"
        "   font-weight: 500;"
        "   background: #E0E0E0;"
        "}"
        "QProgressBar::chunk {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4CAF50, stop:1 #45A049);"
        "   border-radius: 4px;"
        "}"
    );
}

// 槽函数实现
void Dashboard::onRefreshData()
{
    loadSampleData();
    updateStatistics();
    QMessageBox::information(this, "刷新完成", "数据已刷新到最新状态");
}

void Dashboard::onThemeChanged(const QString &theme)
{
    QString themeFile;
    if (theme.contains("深色")) {
        themeFile = ":/assets/themes/dark-theme.qss";
    } else if (theme.contains("浅色")) {
        themeFile = ":/assets/themes/light-theme.qss";
    } else {
        themeFile = ":/assets/themes/modern-blue.qss";
    }

    QFile file(themeFile);
    if (file.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}

void Dashboard::onExportReport()
{
    QMessageBox::information(this, "导出报告", "报告导出功能正在开发中...");
}

void Dashboard::onSettingsClicked()
{
    QMessageBox::information(this, "系统设置", "设置功能正在开发中...");
}

void Dashboard::updateStatistics()
{
    // 模拟数据更新
    int users = QRandomGenerator::global()->bounded(1200, 1300);
    int projects = QRandomGenerator::global()->bounded(50, 60);
    int completion = QRandomGenerator::global()->bounded(70, 85);

    m_totalUsersLabel->setText(QString::number(users));
    m_activeProjectsLabel->setText(QString::number(projects));
    m_completionRateLabel->setText(QString("%1%").arg(completion));
    m_projectProgress->setValue(completion);
}
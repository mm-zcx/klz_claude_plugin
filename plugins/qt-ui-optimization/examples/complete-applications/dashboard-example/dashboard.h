#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QTableWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QSplitter>
#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QFrame>
#include <QTimer>

/**
 * @brief 现代仪表盘主窗口类
 * 展示Qt UI优化技能的应用效果
 */
class Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private slots:
    void onRefreshData();
    void onThemeChanged(const QString &theme);
    void onExportReport();
    void onSettingsClicked();
    void updateStatistics();

private:
    void setupUI();
    void setupHeader();
    void setupSidebar();
    void setupMainContent();
    void setupStatusBar();
    void connectSignals();
    void loadSampleData();

    // UI组件
    QWidget *m_centralWidget;
    QHBoxLayout *m_mainLayout;

    // 头部区域
    QWidget *m_headerWidget;
    QHBoxLayout *m_headerLayout;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QPushButton *m_refreshButton;
    QPushButton *m_exportButton;
    QPushButton *m_settingsButton;
    QComboBox *m_themeCombo;

    // 侧边栏
    QWidget *m_sidebarWidget;
    QVBoxLayout *m_sidebarLayout;
    QGroupBox *m_statisticsGroup;
    QLabel *m_totalUsersLabel;
    QLabel *m_activeProjectsLabel;
    QLabel *m_completionRateLabel;
    QProgressBar *m_projectProgress;

    // 主内容区域
    QSplitter *m_contentSplitter;
    QTabWidget *m_tabWidget;

    // 数据表格标签页
    QWidget *m_tableTab;
    QTableWidget *m_dataTable;

    // 图表标签页
    QWidget *m_chartTab;
    QWidget *m_chartContainer;

    // 控制面板标签页
    QWidget *m_controlTab;
    QScrollArea *m_controlScrollArea;
    QWidget *m_controlWidget;

    // 统计信息
    QTimer *m_updateTimer;

    // 样式相关
    void applyCardStyle(QWidget *widget);
    void applyButtonStyle(QPushButton *button, const QString &styleClass = "");
    void createModernProgressBar(QProgressBar *progressBar, int value = 0);
};

#endif // DASHBOARD_H
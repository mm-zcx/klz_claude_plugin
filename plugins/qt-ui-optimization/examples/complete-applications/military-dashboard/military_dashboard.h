#ifndef MILITARY_DASHBOARD_H
#define MILITARY_DASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QTimer>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QTextEdit>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QHeaderView>

class MilitaryDashboard : public QMainWindow
{
    Q_OBJECT

public:
    MilitaryDashboard(QWidget *parent = nullptr);
    ~MilitaryDashboard();

private slots:
    void updateRadarData();
    void updateSystemStatus();
    void onTacticalAction();
    void onSystemScan();
    void onEmergencyStop();
    void showSystemInfo();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupRadarPanel();
    void setupControlPanel();
    void setupStatusPanel();
    void setupDataPanel();

    // UI组件
    QWidget *m_centralWidget;
    QTabWidget *m_tabWidget;

    // 雷达面板
    QGroupBox *m_radarGroup;
    QWidget *m_radarDisplay;
    QLabel *m_radarStatus;
    QPushButton *m_scanButton;

    // 控制面板
    QGroupBox *m_controlGroup;
    QPushButton *m_tacticalButton;
    QPushButton *m_emergencyButton;
    QPushButton *m_systemButton;
    QSlider *m_powerSlider;
    QLabel *m_powerLabel;
    QComboBox *m_modeCombo;

    // 状态面板
    QGroupBox *m_statusGroup;
    QProgressBar *m_systemProgress;
    QProgressBar *m_communicationProgress;
    QProgressBar *m_weaponProgress;
    QLabel *m_systemLabel;
    QLabel *m_communicationLabel;
    QLabel *m_weaponLabel;

    // 数据面板
    QGroupBox *m_dataGroup;
    QTableWidget *m_dataTable;
    QTextEdit *m_logDisplay;

    // 状态栏组件
    QLabel *m_statusLabel;
    QLabel *m_timeLabel;
    QLabel *m_connectionLabel;

    // 定时器
    QTimer *m_radarTimer;
    QTimer *m_statusTimer;

    // 状态变量
    int m_radarAngle;
    bool m_isScanning;
};

#endif // MILITARY_DASHBOARD_H
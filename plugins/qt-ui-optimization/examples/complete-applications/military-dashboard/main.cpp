#include <QApplication>
#include "military_dashboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    QApplication::setApplicationName("军工仪表盘");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("军工系统");

    // 加载军工主题
    QFile file(":/assets/themes/military-camouflage.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        app.setStyleSheet(styleSheet);
        file.close();
    }

    // 创建主窗口
    MilitaryDashboard dashboard;
    dashboard.show();

    return app.exec();
}
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "dashboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("Qt UI优化示例 - 现代仪表盘");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Qt UI优化技能");

    // 应用样式主题
    QFile styleFile(":/assets/themes/modern-blue.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        app.setStyleSheet(style);
        styleFile.close();
    }

    // 设置应用程序图标
    app.setWindowIcon(QIcon(":/assets/icons/modem/app-icon.png"));

    // 创建并显示主窗口
    Dashboard window;
    window.show();

    return app.exec();
}
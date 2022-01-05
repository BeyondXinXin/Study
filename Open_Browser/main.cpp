// 01 Frame includes
#include "quihelper.h"
#include "quiapplication.h"
#include "commandlinemanager.h"
#include "openbrowserapplication.h"

// VTK includes
#include <vtkOutputWindow.h>
#include <QVTKOpenGLWidget.h>
#include <vtkOpenGLRenderWindow.h>



int main(int argc, char *argv[]) {
    // 主线程名称
    QThread::currentThread()->setObjectName("Main Thread");

    // 设置window渲染方式
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(8);
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    // 信号槽传递类型注册
    // qRegisterMetaType<QList<QList<double>>>("QList<QList<double>>");

    // qt全局初始化
    QUIApplication a(argc, argv);
    if (!a.lock()) {
        QUIHelper::ShowMessageBoxError("正在运行，请不要重复启动", 5, true);
        return 0;
    }

    // 命令行参数设置
    CommandLineManager::Initial(a);

    // 启动主程序
    openBrowserApplication openBrowser_application;
    if (openBrowser_application.Run()) {
    } else {
        return 0;
    }
    return a.exec();
}


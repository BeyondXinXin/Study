#include "dialog.h"
#include <QuickEvent>
#include <QFile>


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QUICK_INSTALL_DETAILED()// QUICK_INSTALL  QUICK_INSTALL_DETAILED
    QUICK_SETSTYLE("../../Examples/resource/Style/gray_style.qss")
    Dialog dialog;
    dialog.show();
    return a.exec();
}

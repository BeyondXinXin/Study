#include "userwork.h"
#include "quickapplication.h"

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QTimer>

UserWork::UserWork(QuickWork *parent) : QuickWork(parent)
{
    QuickApplication::subscibeEvent(this, "linggongju");
}

void UserWork::event_linggongju(const QString &name)
{
     qDebug() << name << "领工具中...";

     QTimer timer;
     QEventLoop loop;
     connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
     timer.start(2000);
     loop.exec();

     qDebug() << "领好工具";
}

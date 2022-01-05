#include "testwork.h"

#include "quickapplication.h"
#include "quickevent.h"

#include <QEventLoop>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>

#include <QJsonObject>

TestWork::TestWork(QuickWork *parent) : QuickWork(parent) {
//    QuickApplication::subscibeEvent(this, "denglu");
    this->Set_move_type_(WorkThread);
    this->Set_run_flag_(1);
    QUICK_SUBSCIBE_L("denglu", 10)
    qDebug() << "--------1200-------";
    QuickApplication::subscibeEvent(this, "dashao");
    QTimer::singleShot(2000, this, []() {
        auto time = QDateTime::currentDateTime();
        QuickApplication::publishEvent("show_time", Qt::AutoConnection, time);
    });
}

void TestWork::event_dashao(const QString &name, QString work) {
    QuickApplication::publishEvent("linggongju", Qt::AutoConnection, name);
    qDebug() <<  name << ":" << work;
}

void TestWork::event_denglu(const Student &stu) {
    qDebug() << "------------**-------------";
}

void TestWork::start() {
    while(Get_run_flag_()) {
        qDebug() << "123123123123";
        QTimer timer;
        QEventLoop loop;
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        connect(this, &QuickWork::quit, &loop, &QEventLoop::quit);
        timer.start(1000);
        loop.exec();
    }
}

//void TestWork::start() {
//    while(this->Get_run_flag_()) {
//        qDebug() << "123123123123";
//        QTimer timer;
//        QEventLoop loop;
//        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
//        connect(this, &QuickWork::quit, &loop, &QEventLoop::quit);
//        timer.start(1000);
//        loop.exec();
//    }
//}

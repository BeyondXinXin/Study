#include "orderwork.h"

#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <QDateTime>

OrderWork::OrderWork(QuickWork *parent)
    : QuickWork(parent) {
    this->Set_move_type_(MoveThreadType::NewThread);
}

void OrderWork::start() {
    while(this->Get_run_flag_()) {
        QTimer timer;
        QEventLoop loop;
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        connect(this, &QuickWork::quit, &loop, &QEventLoop::quit);
        timer.start(1000);
        loop.exec();
    }
}

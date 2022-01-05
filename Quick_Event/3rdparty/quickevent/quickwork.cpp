#include "quickwork.h"
#include "quickevent.h"

#include <QEvent>
#include <QThread>
#include <QMetaMethod>
#include <QSharedPointer>

#include <QTimer>
#include <QEventLoop>

#include <QDebug>

QuickWork::QuickWork(QObject *parent) : QObject(parent) {
    run_flag_ = true;
    move_type_ =  WorkThread;
}

QuickWork::~QuickWork() {
    run_flag_ = false;
    emit quit();
}

void QuickWork::moveToThread(QThread *thread) {
    QObject::moveToThread(thread);
    connect(thread, &QThread::started, this, &QuickWork::start);
    connect(thread, &QThread::finished, this, &QuickWork::deleteLater);
}

void QuickWork::setRunFlag(int value) {
    run_flag_ = value;
}

QuickWork::MoveThreadType QuickWork::getMoveType() const {
    return move_type_;
}




QuickScript::QuickScript(QObject *parent) : QThread(parent) {
    mutex_ = new QMutex();
    this->InitialResult();
}

QuickScript::~QuickScript() {
    delete mutex_;
}

void QuickScript::InitialResult() {
    this->result_ = false;
    this->termination_ = false;
}

void QuickScript::Wait() {
    mutex_->lock();
    condition_.wait(mutex_);
    mutex_->unlock();
}

void QuickScript::Wake() {
    condition_.wakeAll();
}

void QuickScript::run() {
    QUICK_PUBLISH1("ProcessWidgetType", qint32(0))
    this->InitialResult();
    this->Execute();
    QUICK_PUBLISH1("ProcessWidgetType", qint32(1))
}

void QuickScript::SetResult(bool result) {
    result_ |= result;
}


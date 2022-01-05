#include "quickevent.h"

QEvent::Type QuickEvent::S_QuickEvent = static_cast<QEvent::Type>(QEvent::registerEventType());


QuickEvent::QuickEvent(Type type): QEvent(type) {
}

QuickEvent::QuickEvent(
    QByteArray eventName,
    QList<QSharedPointer<QVariant>> &info, QEvent::Type type)
    : QEvent(type), event_name_(eventName), info_(info) {
}

QuickEvent::~QuickEvent() {
}

QuickEvent &QuickEvent::operator=(const QuickEvent &other) {
    event_name_ = other.event_name_;
    this->info_ = other.info_;
    return *this;
}

QByteArray QuickEvent::eventName() const {
    return event_name_;
}

void QuickEvent::setEventName(const QByteArray &eventName) {
    event_name_ = eventName;
}

void QuickEvent::setInfo(const QList<QSharedPointer<QVariant> > &info) {
    info_ = info;
}

QList<QSharedPointer<QVariant> > QuickEvent::info() const {
    return info_;
}

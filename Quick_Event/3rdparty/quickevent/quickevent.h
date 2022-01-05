#ifndef QUICKEVENT_H
#define QUICKEVENT_H

#include <QEvent>
#include <QObject>
#include <QVariant>
#include <QMetaMethod>
#include <QSharedPointer>
#include "quickconfig.h"

#define  METHODHEAD "event_"

class LIBRARYSHARED_EXPORT QuickEvent : public QEvent {
  public:
    QuickEvent(Type type = S_QuickEvent);
    QuickEvent(QByteArray eventName, QList<QSharedPointer<QVariant>> &info,
               Type type = S_QuickEvent);
    virtual ~QuickEvent();
    QuickEvent &operator=(const QuickEvent &other);
    QByteArray eventName() const;
    void setEventName(const QByteArray &eventName);
    QList<QSharedPointer<QVariant> > info() const;
    void setInfo(const QList<QSharedPointer<QVariant> > &info);
  public:
    static QEvent::Type S_QuickEvent;
  private:
    QByteArray event_name_;
    QList<QSharedPointer<QVariant>> info_;
};

#endif // QUICKEVENT_H

#ifndef QUICKAPPLICATION_H
#define QUICKAPPLICATION_H

#include <QApplication>
#include <QObject>
#include <QSet>
#include <QMap>
#include <QReadWriteLock>
#include <QByteArray>
#include <QVariant>
#include <QSharedPointer>
#include <QThread>
#include <QDebug>

#if defined(Q_OS_LINUX)
    #include <cxxabi.h>
#endif
#include "quickevent.h"
#include "quickconfig.h"


class QuickWork;

class LIBRARYSHARED_EXPORT QuickApplication : public QObject {
    Q_OBJECT
  public:
    template<class T>
    static void getList(QList<QByteArray> &typeNames,
                        QList<QGenericArgument> &list, T &&t) {
#if defined(Q_OS_LINUX)
        char *type = abi::__cxa_demangle(typeid(t).name(), nullptr, nullptr, nullptr);
        typeNames << QByteArray(type);
        list << Q_ARG(T, t);
        free(type);
#elif defined(Q_OS_WIN)
        typeNames << QByteArray(typeid(t).name());
        list << Q_ARG(T, t);
#endif
    }
    template<class T>
    static void getList(QList<QByteArray> &typeNames,
                        QList< QSharedPointer<QVariant> > &list, T &&t) {
#if defined(Q_OS_LINUX)
        char *type = abi::__cxa_demangle(typeid(t).name(), nullptr, nullptr, nullptr);
        typeNames << QByteArray(type);
        QSharedPointer<QVariant> ptr(new QVariant());
        ptr->setValue(t);
        list << ptr;
        free(type);
#elif defined(Q_OS_WIN)
        typeNames << QByteArray(typeid(t).name());
        QSharedPointer<QVariant> ptr(new QVariant());
        ptr->setValue(t);
        list << ptr;
#endif
    }
    template<class ...Args>
    static void publishEvent(QByteArray eventName, Qt::ConnectionType type, Args &&... args) {
        if(show_detailed_) {
            qDebug() << "\n-----------------"
                     << "publishEvent:-----------------"
                     << "\nEvent name: " << eventName
                     << "\nEvent ConnectionType:" << type
                     << "\nEvent Args:" << type
                     << "\nEvent Thread:" << QThread::currentThread()
                     << "\n----------------------------------\n";
        }
        if(eventName.isEmpty()) {
            return ;
        }
        auto argsNum = sizeof...(args);
        Q_ASSERT_X(argsNum <= 10, "QuickEvent", "publishEvent argv number not greater than 10");
        QReadLocker loker(&s_lock);
        if(s_quick_event_pool_high.contains(eventName) ||
                s_quick_event_pool.contains(eventName) ||
                s_quick_event_pool_low.contains(eventName)) {
            auto list = s_quick_event_pool_high[eventName].values();
            list.append(s_quick_event_pool[eventName].values());
            list.append(s_quick_event_pool_low[eventName].values());
            auto methodName = QByteArray(METHODHEAD) + eventName;
            foreach (auto var, list) {
                if(var) {
                    qDebug() << var;
                } else {
                    qDebug() << "---------------";
                }
                if(var->thread() == QThread::currentThread() ||
                        type == Qt::BlockingQueuedConnection) {
                    // 同一个线程或者需要阻塞
                    QList<QGenericArgument> list;
                    QList<QByteArray> typeNames;
                    std::initializer_list<int32_t> {(getList(typeNames, list, args), 0)...};
                    while(list.size() < 10) {
                        list << QGenericArgument();
                    }
                    auto index = -1;
                    if((index = methodIndex(var, typeNames, argsNum, methodName)) == -1) {
                        continue;
                    }
                    // 防止死锁
                    if(var->thread() == QThread::currentThread()) {
                        var->metaObject()->method(index).invoke(
                            var, Qt::DirectConnection, list[0], list[1], list[2], list[3],
                            list[4], list[5], list[6], list[7], list[8], list[9]);
                    } else {
                        var->metaObject()->method(index).invoke(
                            var, type, list[0], list[1], list[2], list[3],
                            list[4], list[5], list[6], list[7], list[8], list[9]);
                    }
                } else {
                    // 夸线程不需要阻塞
                    // 把这个signal包装成一个 QEvent，放到消息队列里。
                    // postEvent 非阻塞发送
                    QList< QSharedPointer<QVariant> > list;
                    QList<QByteArray> typeNames;
                    std::initializer_list<int32_t> {(getList(typeNames, list, args), 0)...};
                    QuickEvent *event = new QuickEvent();
                    event->setInfo(list);
                    event->setEventName(eventName);
                    QApplication::postEvent(var, event);
                }
            }
        }
    }
    static int methodIndex(QObject *recv, QList<QByteArray> &typeNames,
                           quint64 argsNum, QByteArray methodName);
    static bool subscibeEvent(
        QObject *listener, QByteArray eventName, const qint32 &level = -1);
    static bool UnsubscribeEvent(QObject *listener, QByteArray eventName);
    static bool UnsubscribeEvent(QObject *listener);
    static void exit(int rlt = 0);
    static bool show_detailed_;
  private:
    static QMap < QByteArray, QMap<qint32, QObject *> > s_quick_event_pool_high;
    static QMap < QByteArray, QMap<qint32, QObject *> > s_quick_event_pool;
    static QMap < QByteArray, QMap<qint32, QObject *> > s_quick_event_pool_low;
    static QReadWriteLock s_lock;

};

#endif // QUICKAPPLICATION_H

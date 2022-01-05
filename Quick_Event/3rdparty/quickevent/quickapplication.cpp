#include "quickapplication.h"
#include "quickevent.h"
#include "quickwork.h"
#include "quickcontroller.h"

#include <QObject>
#include <QTimer>
#include <QEventLoop>

QMap < QByteArray, QMap<qint32, QObject *> > QuickApplication::s_quick_event_pool_high;
QMap < QByteArray, QMap<qint32, QObject *> > QuickApplication::s_quick_event_pool;
QMap < QByteArray, QMap<qint32, QObject *> > QuickApplication::s_quick_event_pool_low;

QReadWriteLock QuickApplication::s_lock;
bool QuickApplication::show_detailed_ = false;



void typeName(const QByteArray &s, QByteArray &d) {
    foreach (auto var, s) {
        if((var >= 'A' && var <= 'Z') ||
                (var >= 'a' && var <= 'z') ||
                (var >= '0' && var <= '9') ||
                var == '_') {
            d.append(var);
            continue;
        }
        break;
    }
}

bool checkpParameter(QList<QByteArray> s, QList<QByteArray> d) {
    for(int i = 0; i < d.size(); ++ i) {
        QByteArray name;
        typeName(d[i], name);
        if(name.isEmpty()) {
            return  false;
        }
        if(!(s[i].startsWith(name) || s[i].startsWith("class " + name))) {
            return false;
        }
    }
    return true;
}

int QuickApplication::methodIndex(QObject *recv, QList<QByteArray> &typeNames,
                                  quint64 argsNum, QByteArray methodName) {
    if(recv == nullptr) {
        return -1;
    }
    auto obj = recv->metaObject();
    for(int i = obj->methodOffset(); i < recv->metaObject()->methodCount(); ++ i) {
        auto method = obj->method(i);
        if(methodName == method.name()) {
            if(static_cast<unsigned int>(method.parameterCount()) > argsNum) {
                continue;
            }
            if(checkpParameter(typeNames, method.parameterTypes())) {
                return i;
            }
        }
    }
    return -1;
}

//
bool QuickApplication::subscibeEvent(
    QObject *listener, QByteArray eventName, const qint32 &level) {
    if(show_detailed_) {
        if(level < 0) {
            qDebug() << "\n-----------------"
                     << "subscibeEvent:-----------------"
                     << "\nlistener:" << listener
                     << "\nEvent name:" << eventName
                     << "\nsubscibe level" << "Disorder"
                     << "\nEvent Thread" << QThread::currentThread()->objectName()
                     << "\n----------------------------------\n";
        } else {
            qDebug() << "\n-----------------"
                     << "subscibeEvent:-----------------"
                     << "\nlistener:" << listener
                     << "\nEvent name:" << eventName
                     << "\nsubscibe level" << level
                     << "\nEvent Thread" << QThread::currentThread()->objectName()
                     << "\n----------------------------------\n";
        }
    }
    if(level < 0) {
        // 无序链接
        QWriteLocker loker(&s_lock);
        if(s_quick_event_pool.contains(eventName)) {
            QMap<qint32, QObject *>tmplist = s_quick_event_pool[eventName];
            tmplist.insert(tmplist.size(), listener);
            s_quick_event_pool.insert(eventName, tmplist);
            return true;
        }
        QMap<qint32, QObject *>tmplist;
        tmplist.insert(0, listener);
        s_quick_event_pool.insert(eventName, tmplist);
    } else if(level >= 0 && level < 100) {
        // 高优先级
        QWriteLocker loker(&s_lock);
        if(s_quick_event_pool_high.contains(eventName)) {
            QMap<qint32, QObject *>tmplist = s_quick_event_pool[eventName];
            tmplist.insert(level, listener);
            s_quick_event_pool.insert(eventName, tmplist);
            return true;
        }
        QMap<qint32, QObject *>tmplist;
        tmplist.insert(level, listener);
        s_quick_event_pool_high.insert(eventName, tmplist);
    } else if(level >= 100) {
        // 低优先级
        QWriteLocker loker(&s_lock);
        if(s_quick_event_pool_low.contains(eventName)) {
            QMap<qint32, QObject *>tmplist = s_quick_event_pool[eventName];
            tmplist.insert(level, listener);
            s_quick_event_pool.insert(eventName, tmplist);
            return true;
        }
        QMap<qint32, QObject *>tmplist;
        tmplist.insert(level, listener);
        s_quick_event_pool_low.insert(eventName, tmplist);
    }
    return true;
}

bool QuickApplication::UnsubscribeEvent(QObject *listener, QByteArray eventName) {
    QWriteLocker loker(&s_lock);
    bool result = false;
    if(s_quick_event_pool.contains(eventName)) {
        QMap<qint32, QObject *> tmplist = s_quick_event_pool[eventName];
        tmplist.remove(tmplist.key(listener));
        s_quick_event_pool.insert(eventName, tmplist);
        result = true;
    }
    if(s_quick_event_pool_high.contains(eventName)) {
        QMap<qint32, QObject *> tmplist = s_quick_event_pool_high[eventName];
        tmplist.remove(tmplist.key(listener));
        s_quick_event_pool.insert(eventName, tmplist);
        result = true;
    }
    if(s_quick_event_pool_low.contains(eventName)) {
        QMap<qint32, QObject *> tmplist = s_quick_event_pool_low[eventName];
        tmplist.remove(tmplist.key(listener));
        s_quick_event_pool.insert(eventName, tmplist);
        result = true;
    }
    return result;
}

bool QuickApplication::UnsubscribeEvent(QObject *listener) {
    QWriteLocker loker(&s_lock);
    foreach (auto var, s_quick_event_pool.keys()) {
        s_quick_event_pool[var].remove(s_quick_event_pool[var].key(listener));
    }
    foreach (auto var, s_quick_event_pool_low.keys()) {
        s_quick_event_pool_low[var].remove(s_quick_event_pool_low[var].key(listener));
    }
    foreach (auto var, s_quick_event_pool_high.keys()) {
        s_quick_event_pool_high[var].remove(s_quick_event_pool_high[var].key(listener));
    }
    return true;
}


void QuickApplication::exit(int rlt) {
    QuickController::destory();
    qApp->exit(rlt);
}

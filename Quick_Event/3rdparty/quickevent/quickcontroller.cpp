#include "quickcontroller.h"
#include "quickwork.h"

QMap<QThread *, QuickWork *> QuickController::threads_;// 新线程
QPair<QThread *, QSet<QuickWork *> > QuickController::workthread_;// 工作线程
QSet<QuickWork *> QuickController::works_;// 主线程
bool QuickController::destory_flag_ = false;

QuickController::QuickController(QObject *parent) : QObject(parent) {
    QThread::currentThread()->setObjectName("Main Thread");
    auto list = static_cast< QList<QByteArray> *>(NewInstance(""));
    if(QuickApplication::show_detailed_ && !list->isEmpty()) {
        qDebug() << "\n-----------------"
                 << "QuickController Initialization:-----------------"
                 << "\nLib Name:" << QString::fromStdString(QuickEvent_NAME)
                 << "\nLib VER:" << QString::fromStdString(QuickEvent_VER)
                 << "\nControlle QuickWorks:" << list[0]
                 << "\n----------------------------------\n";
    }
    auto workt = new QThread(this);// 工作线程
    workt->setObjectName("Work Thread");
    workthread_.first = nullptr;
    QSet<QuickWork *> set;
    foreach(auto var, *list) {
        auto type = QMetaType::type(var + "*");
        auto meta = QMetaType::metaObjectForType(type);
        auto obj = meta->newInstance(Q_ARG(QuickWork *, nullptr));
        auto work = qobject_cast<QuickWork *>(obj);
        if(work == nullptr) {
            continue;
        }
        QThread *t = nullptr;
        switch (work->getMoveType()) {
            case QuickWork::NewThread:
                t = new QThread(this);
                work->moveToThread(t);
                threads_.insert(t, work);
                t->start();
                break;
            case QuickWork::WorkThread:
                set.insert(work);
                work->moveToThread(workt);
                break;
            case QuickWork::MainThread:
                connect(this, &QuickController::startwork, work, &QuickWork::start);
                works_.insert(work);
                break;
        }
    }
    if(!set.isEmpty()) {
        workthread_.first = workt;
        workthread_.second = set;
        workt->start();
    } else {
        workt->deleteLater();
    }
    if(!works_.isEmpty()) {
        emit startwork();
    }
}

QuickController::~QuickController() {
    if(!destory_flag_) {
        destory();
    }
}

void *QuickController::NewInstance(
    const QByteArray &name, const
    QuickController::AuotOrder &order,
    const qint32 &key) {
    // High、Low 手动注册顺序，如果重复顺序则会覆盖之前的
    static QSet<QByteArray> set_printed;
    static QSet<QByteArray> set;
    static QMap<qint32, QByteArray> high_map;
    static QMap<qint32, QByteArray> low_map;
    static QList<QByteArray> tmp_value;
    if(!name.isEmpty()) {
        // 输入不为空，注册
        switch (order) {
            case High:// 有序，优先级高
                if(high_map.keys().contains(key) &&
                        high_map.value(key) != name &&
                        !set_printed.contains(name)) {
                    // 覆盖之前注册
                    set_printed << name << high_map.value(key);
                    qDebug() << "\n-----------------"
                             << "QuickController OrderHigh cover:-----------------"
                             << "\ncover key:" << key
                             << "\ncover name:" << high_map.value(key) << name
                             << "\n----------------------------------\n";
                }
                high_map.insert(key, name);
                break;
            case Disorder:// 无序，优先级中
                set.insert(name);
                break;
            case Low:// 有序，优先级低
                if(low_map.keys().contains(key) &&
                        low_map.value(key) != name &&
                        !set_printed.contains(name)) {
                    // 覆盖之前注册
                    set_printed << name << low_map.value(key);
                    qDebug() << "\n-----------------"
                             << "QuickController OrderLow cover:-----------------"
                             << "\ncover key:" << key
                             << "\ncover name:" << low_map.value(key) << name
                             << "\n----------------------------------\n";
                }
                low_map.insert(key, name);
                break;
        }
        return nullptr;
    } else {
        // 输入为空，获取注册信息
        tmp_value.clear();
        tmp_value << high_map.values() << set.toList() << low_map.values();
        return static_cast<void *>(&tmp_value);
    }
}


void QuickController::destory() {
    auto it = threads_.begin();
    for(; it != threads_.end(); ++ it) {
        it.value()->setRunFlag(false);
        it.key()->exit();
        it.key()->wait(5);
    }
    foreach(auto var, workthread_.second) {
        var->setRunFlag(false);
    }
    if(workthread_.first != nullptr) {
        workthread_.first->exit();
        workthread_.first->wait(5);
    }
    // 主线程析构
    foreach(auto var, works_) {
        var->setRunFlag(false);
        delete var;
    }
    destory_flag_ = true;
}

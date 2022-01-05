/**
 * @brief main 发布订阅
 * 多种发布展示（同步、异步、异步等待、自动）
 * 小心死锁，尤其是本线程同步、跨线程阻塞同步
 */

#include <QuickEvent>
#include <QFile>
#include <QTimer>

#define Establish(Classname)\
    public:\
    virtual ~Classname() override {\
        qDebug() << QString(#Classname)+"  End of deconstruction";\
    }\
    private:\


class NewThreadSubscibe1 : public QuickWork {
    Q_OBJECT
    Establish(NewThreadSubscibe1)
  public:
    Q_INVOKABLE explicit NewThreadSubscibe1(
        QuickWork *parent = nullptr): QuickWork(parent) {
        QUICK_SUBSCIBE("Example5")
        this->Set_move_type_(NewThread);
        this->Set_run_flag_(false);
    }
  private Q_SLOTS:
    void event_Example5(const QString &) {
        qDebug() << "New1 Subscibe bugin" << QThread::currentThread();
        QThread::msleep(300);
        qDebug() << "New1 Subscibe end" << QThread::currentThread();
    }
};
QUICK_AUTO(NewThreadSubscibe1)

class NewThreadSubscibe2 : public QuickWork {
    Q_OBJECT
    Establish(NewThreadSubscibe2)
  public:
    Q_INVOKABLE explicit NewThreadSubscibe2(
        QuickWork *parent = nullptr): QuickWork(parent) {
        QUICK_SUBSCIBE("Example5")
        this->Set_move_type_(NewThread);
        this->Set_run_flag_(false);
    }
  private Q_SLOTS:
    void event_Example5(const QString &) {
        qDebug() << "New2 Subscibe bugin" << QThread::currentThread();
        QThread::msleep(300);
        qDebug() << "New2 Subscibe end" << QThread::currentThread();
    }
};
QUICK_AUTO(NewThreadSubscibe2)



int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QUICK_INSTALL_DETAILED()// QUICK_INSTALL  QUICK_INSTALL_DETAILED
    QTimer::singleShot(1000, &a, [ = ] {
        QThread::msleep(2000);
        qDebug() << "\n-------------Direct PUBLISH-------------";
        QUICK_Direct_PUBLISH1("Example5", QString(""))
        QThread::msleep(2000);
        qDebug() << "\n-------------Queued PUBLISH-------------";
        QUICK_Queued_PUBLISH1("Example5", QString(""))
        QThread::msleep(2000);
        qDebug() << "\n-------------Blocking Queued PUBLISH-------------";
        QUICK_BlockingQueued_PUBLISH1("Example5", QString(""))
        qDebug() << "\n-------------Auto PUBLISH1-------------";
        QThread::msleep(2000);
        QUICK_PUBLISH1("Example5", QString(""))
    });
    QTimer::singleShot(10000, &a, [ = ] {
        qDebug() << "-------------Quit-------------";
        QuickApplication::exit(0);
    });
    return a.exec();
}

#include "main.moc"

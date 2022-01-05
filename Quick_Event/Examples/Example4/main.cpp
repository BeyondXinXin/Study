/**
 * @brief main 发布订阅
 * 跨线程发布订阅
 * WorkThread的start耗时操作会阻塞所有WorkThread中订阅事件
 */

#include <QuickEvent>
#include <QFile>
#include <QTimer>


class MainThreadSubscibe : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit MainThreadSubscibe(
        QuickWork *parent = nullptr): QuickWork(parent) {
        QUICK_SUBSCIBE("Example4")
        this->Set_move_type_(MainThread);
        this->Set_run_flag_(false);
    }
  private Q_SLOTS:
    void event_Example4(const QString &) {
        qDebug() << "MainThreadSubscibe" << QThread::currentThread();
    }
};
QUICK_AUTO(MainThreadSubscibe)

class WorkThreadSubscibe : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit WorkThreadSubscibe(
        QuickWork *parent = nullptr): QuickWork(parent) {
        QUICK_SUBSCIBE("Example4")
        this->Set_move_type_(WorkThread);
        this->Set_run_flag_(false);
    }
  private Q_SLOTS:
    void event_Example4(const QString &) {
        qDebug() << "WorkThreadSubscibe" << QThread::currentThread();
    }
};
QUICK_AUTO(WorkThreadSubscibe)

class NewThreadSubscibe : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit NewThreadSubscibe(
        QuickWork *parent = nullptr): QuickWork(parent) {
        QUICK_SUBSCIBE("Example4")
        this->Set_move_type_(NewThread);
        this->Set_run_flag_(false);
    }
  private Q_SLOTS:
    void event_Example4(const QString &) {
        qDebug() << "NewThreadSubscibe" << QThread::currentThread();
    }
};
QUICK_AUTO(NewThreadSubscibe)

class MainThreadPublish : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit MainThreadPublish(
        QuickWork *parent = nullptr): QuickWork(parent) {
        this->Set_move_type_(MainThread);
        this->Set_run_flag_(true);
    }
    void start() {
        while(this->Get_run_flag_()) {
            this->Set_run_flag_(false);
            QThread::msleep(1000);
            qDebug() << "\n-------------Main Thread Publish-------------";
            QUICK_PUBLISH1("Example4", QString(""))
        }
    }
};
QUICK_AUTO(MainThreadPublish)

class WorkThreadPublish : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit WorkThreadPublish(
        QuickWork *parent = nullptr): QuickWork(parent) {
        this->Set_move_type_(WorkThread);
        this->Set_run_flag_(true);
    }
    void start() {
        while(this->Get_run_flag_()) {
            this->Set_run_flag_(false);
            QThread::msleep(3000);
            qDebug() << "\n-------------Work Thread Publish-------------";
            QUICK_PUBLISH1("Example4", QString(""))
        }
    }
};
QUICK_AUTO(WorkThreadPublish)

class NewThreadPublish : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit NewThreadPublish(
        QuickWork *parent = nullptr): QuickWork(parent) {
        this->Set_move_type_(NewThread);
        this->Set_run_flag_(true);
    }
    void start() {
        while(this->Get_run_flag_()) {
            this->Set_run_flag_(false);
            QThread::msleep(5000);
            qDebug() << "\n-------------New Thread Publish-------------";
            QUICK_PUBLISH1("Example4", QString(""))
        }
    }
};
QUICK_AUTO(NewThreadPublish)

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QUICK_INSTALL_DETAILED()// QUICK_INSTALL  QUICK_INSTALL_DETAILED
    QTimer::singleShot(7000, &a, [ = ] {
        qDebug() << "-------------Quit-------------";
        qApp->quit();
    });
    return a.exec();
}

#include "main.moc"

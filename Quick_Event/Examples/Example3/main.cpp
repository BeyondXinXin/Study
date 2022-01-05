/**
 * @brief main 自动实例化 QuickApplication、QuickWork
 * 线程归属展示
 * WorkThread下线程类似MainThread，阻塞依次执行
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


class Disorder0 : public QuickWork {
    Q_OBJECT
    Establish(Disorder0)
  public:
    Q_INVOKABLE explicit Disorder0( QuickWork *parent = nullptr): QuickWork(parent) {
        this->Set_move_type_(MainThread);
        this->Set_run_flag_(true);
        qDebug() << "Disorder0 Initialization" << QThread::currentThread();
    }\
    void start() override {
        while(this->Get_run_flag_()) {
            qDebug() << "Disorder0 Run" << QThread::currentThread();
            this->Set_run_flag_(false);
        }
    }
};
QUICK_AUTO(Disorder0)

class Disorder1 : public QuickWork {
    Q_OBJECT
    Establish(Disorder1)
  public:
    Q_INVOKABLE explicit Disorder1( QuickWork *parent = nullptr)
        : QuickWork(parent) {
        this->Set_move_type_(WorkThread);
        this->Set_run_flag_(true);
        qDebug() << "Disorder1 Initialization" << QThread::currentThread();
    }
    void start() override {
        while(this->Get_run_flag_()) {
            qDebug() << "Disorder1 Run msleep Begin" << QThread::currentThread();
            this->Set_run_flag_(false);
            QThread::msleep(1000);
            qDebug() << "Disorder1 Run msleep End" << QThread::currentThread();
        }
    }
};
QUICK_AUTO(Disorder1)

class Disorder2 : public QuickWork {
    Q_OBJECT
    Establish(Disorder2)
  public:
    Q_INVOKABLE explicit Disorder2( QuickWork *parent = nullptr)
        : QuickWork(parent) {
        this->Set_move_type_(NewThread);
        this->Set_run_flag_(true);
        qDebug() << "Disorder2 Initialization" << QThread::currentThread();
    }
    void start() override {
        while(this->Get_run_flag_()) {
            qDebug() << "Disorder2 Run" << QThread::currentThread();
            this->Set_run_flag_(false);
        }
    }
};
QUICK_AUTO(Disorder2)

class Disorder3 : public QuickWork {
    Q_OBJECT
    Establish(Disorder3)
  public:
    Q_INVOKABLE explicit Disorder3( QuickWork *parent = nullptr)
        : QuickWork(parent) {
        this->Set_move_type_(WorkThread);
        this->Set_run_flag_(true);
        qDebug() << "Disorder3 Initialization" << QThread::currentThread();
    }
    void start() override {
        while(this->Get_run_flag_()) {
            qDebug() << "Disorder3 Run msleep Begin" << QThread::currentThread();
            this->Set_run_flag_(false);
            QThread::msleep(1000);
            qDebug() << "Disorder3 Run msleep End" << QThread::currentThread();
        }
    }
};
QUICK_AUTO(Disorder3)


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QUICK_INSTALL_DETAILED()// QUICK_INSTALL  QUICK_INSTALL_DETAILED
    QTimer::singleShot(5000, &a, [ = ] {
        qDebug() << "-------------quit-------------";
        qApp->quit();
    });
    return a.exec();
}

#include "main.moc"

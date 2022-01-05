/**
 * @brief main
 * 自动实例化 QuickApplication、QuickWork
 * 排序反射展示
 * Disorder反射乱序的
 */

#include <QuickEvent>
#include <QFile>

#define Establish(Classname)\
    public:\
    Q_INVOKABLE explicit Classname( QuickWork *parent = nullptr)\
        : QuickWork(parent) {\
        qDebug() << QString(#Classname)+"  Initialization complete";\
    }\
    virtual ~Classname() override {\
        qDebug() << QString(#Classname)+"  End of deconstruction";\
    }\
    private:\


class High0 : public QuickWork {
    Q_OBJECT
    Establish(High0)
};
QUICK_AUTO_H(High0, 0)

class High1 : public QuickWork {
    Q_OBJECT
    Establish(High1)
};
QUICK_AUTO_H(High1, 1)

class High2 : public QuickWork {
    Q_OBJECT
    Establish(High2)
};
QUICK_AUTO_H(High2, 2)

class Disorder0 : public QuickWork {
    Q_OBJECT
    Establish(Disorder0)
};
QUICK_AUTO(Disorder0)

class Disorder1 : public QuickWork {
    Q_OBJECT
    Establish(Disorder1)
};
QUICK_AUTO(Disorder1)

class Disorder2 : public QuickWork {
    Q_OBJECT
    Establish(Disorder2)
};
QUICK_AUTO(Disorder2)

class Low0 : public QuickWork {
    Q_OBJECT
    Establish(Low0)
};
QUICK_AUTO_L(Low0, 0)

class Low1 : public QuickWork {
    Q_OBJECT
    Establish(Low1)
};
QUICK_AUTO_L(Low1, 1)

class Low2 : public QuickWork {
    Q_OBJECT
    Establish(Low2)
};
QUICK_AUTO_L(Low2, 2)

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QUICK_INSTALL_DETAILED()// QUICK_INSTALL  QUICK_INSTALL_DETAILED
    a.quit();
    return 0;
}

#include "main.moc"

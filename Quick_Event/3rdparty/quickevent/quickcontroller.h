#ifndef QUICKCONTROLLER_H
#define QUICKCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QThread>
#include "quickconfig.h"

class QuickWork;

class LIBRARYSHARED_EXPORT QuickController : public QObject {
    Q_OBJECT
  public:
    typedef enum {
        High = 0,// 有序，优先级高
        Disorder = 1,// 无序，优先级中
        Low = 2,// 有序，优先级低
    } AuotOrder;
  public:
    QuickController(QObject *parent = nullptr);
    ~QuickController();
    static void *NewInstance(const QByteArray &name,
                             const QuickController::AuotOrder &order = Disorder,
                             const qint32 &key = 0);
    static void destory();
  signals:
    void startwork();
  private:
    static QMap<QThread *, QuickWork *> threads_;
    static QPair<QThread *, QSet<QuickWork *> > workthread_;
    static QSet<QuickWork *> works_;
    static bool destory_flag_;
};

#endif // QUICKCONTROLLER_H

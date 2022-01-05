#ifndef ORDERWORK_H
#define ORDERWORK_H

#include "quickwork.h"
#include <QObject>

class OrderWork : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit OrderWork(QuickWork *parent = nullptr);

  signals:

  public slots:
    void start();
};
QUICK_AUTO_H(OrderWork, 2)

#endif // ORDERWORK_H

#ifndef USERWORK_H
#define USERWORK_H

#include "quick.h"
#include "quickwork.h"
#include <QVariant>
#include <QSharedPointer>


class UserWork : public QuickWork {
    Q_OBJECT
  public:
    Q_INVOKABLE explicit UserWork(QuickWork *parent = nullptr);
  public slots:
    void event_linggongju(const QString &name);
};
QUICK_AUTO_L(UserWork, 1)

#endif // USERWORK_H

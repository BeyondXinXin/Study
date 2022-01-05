#ifndef QUICKWORK_H
#define QUICKWORK_H

#include <QObject>
#include <QEvent>

#include "quick.h"
#include "quickconfig.h"

#include <QSet>
#include <QList>
#include <QMutex>
#include <QString>
#include <QArgument>
#include <QWaitCondition>

class LIBRARYSHARED_EXPORT QuickWork : public QObject {
    Q_OBJECT
    QUICK_EVENT(QObject)
  public:
    typedef enum {
        MainThread = 0,
        WorkThread,
        NewThread
    } MoveThreadType;
    explicit QuickWork(QObject *parent = nullptr);
    virtual ~QuickWork() override;
    void moveToThread(QThread *thread);
    void setRunFlag(int value);
    MoveThreadType getMoveType() const;
  Q_SIGNALS:
    void quit();
  public Q_SLOTS:
    virtual void start() {}
  protected:
    QUICK_INITIAL_VAR(move_type_, MoveThreadType)//对象移动方式
    QUICK_INITIAL_VAR(run_flag_, int)
};
Q_DECLARE_METATYPE(QuickWork::MoveThreadType)

class QuickScript : public QThread {
    Q_OBJECT
    QUICK_EVENT(QObject)
  public:
    explicit QuickScript(QObject *parent = nullptr);
    virtual ~QuickScript() override;
    virtual void Execute() = 0;
    virtual bool BuildView() = 0;
    void InitialResult();
    void Wait();
    void Wake();
  Q_SIGNALS:
    void SignalProgressIn(qint32 value);// 脚本进度
  protected:
    virtual void run() override final;
    virtual void Initial() = 0;
    void SetResult(bool result);
  protected:
    QWaitCondition condition_;
    QMutex *mutex_;
    QUICK_INITIAL_VAR(termination_, bool)
    QUICK_INITIAL_VAR(result_, bool)
};


#endif // QUICKWORK_H

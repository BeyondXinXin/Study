#ifndef QUICK_H
#define QUICK_H

#include "quickconfig.h"
#include "quickcontroller.h"
#include "quickapplication.h"

#define QUICK_AUTO(ClassName)\
    Q_DECLARE_METATYPE(ClassName *) \
    static int ClassId##ClassName = qRegisterMetaType<ClassName *>();\
    static void *ThisPtr##ClassName = QuickController::NewInstance(#ClassName);

#define QUICK_AUTO_H(ClassName,value)\
    Q_DECLARE_METATYPE(ClassName *) \
    static int ClassId##ClassName = qRegisterMetaType<ClassName *>();\
    static void *ThisPtr##ClassName = QuickController::NewInstance(\
                                      #ClassName,QuickController::High,value);

#define QUICK_AUTO_L(ClassName,value)\
    Q_DECLARE_METATYPE(ClassName *) \
    static int ClassId##ClassName = qRegisterMetaType<ClassName *>();\
    static void *ThisPtr##ClassName = QuickController::NewInstance(\
                                      #ClassName,QuickController::Low,value);




#define QUICK_EVENT(PARENTANME)\
    public:\
    bool event(QEvent *e) override\
    {\
        if(e->type() == QuickEvent::S_QuickEvent)\
        {\
            auto quickEvent = dynamic_cast<QuickEvent*>(e);\
            auto methodName = quickEvent->eventName();\
            auto infos = quickEvent->info(); \
            QList<QGenericArgument> list; \
            methodName = QByteArray(METHODHEAD) + methodName;\
            for(int i = 0; i < quickEvent->info().size() && i < 10; ++ i)\
                list << QGenericArgument(infos[i]->typeName(), infos[i]->constData());\
            while(list.size() <= 10)\
                list << QGenericArgument();\
            QMetaObject::invokeMethod(this, methodName, list[0], list[1], list[2], list[3],\
                                      list[4], list[5], list[6], list[7], list[8], list[9]);\
            return true;\
        } \
        return PARENTANME::event(e);\
    }

#define QUICK_DESTRUCT \
    QuickApplication::UnsubscribeEvent(this);
#define QUICK_AUTO_DESTRUCT() \
    connect(this,&QObject::destroyed,[=](){\
        QuickApplication::UnsubscribeEvent(this);});

#define QUICK_SUBSCIBE_OBJ(obj,name)\
    connect(obj,&QObject::destroyed,[=](){\
        QuickApplication::UnsubscribeEvent(obj);});\
    QuickApplication::subscibeEvent(obj, name);
#define QUICK_SUBSCIBE(name)\
    QUICK_AUTO_DESTRUCT()\
    QuickApplication::subscibeEvent(this, name);
#define QUICK_SUBSCIBE_H(name,lev)\
    QUICK_AUTO_DESTRUCT()\
    QuickApplication::subscibeEvent(this, name,lev);
#define QUICK_SUBSCIBE_L(name,lev)\
    QUICK_AUTO_DESTRUCT()\
    QuickApplication::subscibeEvent(this, name,100+lev);

#define QUICK_PUBLISH1(name,arg1)\
    QuickApplication::publishEvent(name,Qt::AutoConnection, arg1);
#define QUICK_PUBLISH2(name,arg1,arg2)\
    QuickApplication::publishEvent(name,Qt::AutoConnection, arg1,arg2);
#define QUICK_PUBLISH3(name,arg1,arg2,arg3)\
    QuickApplication::publishEvent(name,Qt::AutoConnection, arg1,arg2,arg3);
#define QUICK_PUBLISH4(name,arg1,arg2,arg3,arg4)\
    QuickApplication::publishEvent(name,Qt::AutoConnection, arg1,arg2,arg3,arg4);

#define QUICK_Direct_PUBLISH1(name,arg1)\
    QuickApplication::publishEvent(name,Qt::DirectConnection, arg1);
#define QUICK_Direct_PUBLISH2(name,arg1,arg2)\
    QuickApplication::publishEvent(name,Qt::DirectConnection, arg1,arg2);
#define QUICK_Direct_PUBLISH3(name,arg1,arg2,arg3)\
    QuickApplication::publishEvent(name,Qt::DirectConnection, arg1,arg2,arg3);

#define QUICK_Queued_PUBLISH1(name,arg1)\
    QuickApplication::publishEvent(name,Qt::QueuedConnection, arg1);
#define QUICK_Queued_PUBLISH2(name,arg1,arg2)\
    QuickApplication::publishEvent(name,Qt::QueuedConnection, arg1,arg2);
#define QUICK_Queued_PUBLISH3(name,arg1,arg2,arg3)\
    QuickApplication::publishEvent(name,Qt::QueuedConnection, arg1,arg2,arg3);

#define QUICK_BlockingQueued_PUBLISH1(name,arg1)\
    QuickApplication::publishEvent(name,Qt::BlockingQueuedConnection, arg1);
#define QUICK_BlockingQueued_PUBLISH2(name,arg1,arg2)\
    QuickApplication::publishEvent(name,Qt::BlockingQueuedConnection, arg1,arg2);
#define QUICK_BlockingQueued_PUBLISH3(name,arg1,arg2,arg3)\
    QuickApplication::publishEvent(name,Qt::BlockingQueuedConnection, arg1,arg2,arg3);

#define QUICK_GETSET(name,type) \
    virtual void Set_##name (const type &_arg){this->name = _arg;} \
    virtual type Get_##name () const{return this->name;}

#define QUICK_INITIAL_VAR(name,type) \
    public:\
    virtual void Set_##name (const type &_arg){this->name = _arg;} \
    virtual type Get_##name () const{return this->name;}\
    private:\
    type name;


#define QUICK_GETSET_Object(name,type)\
    virtual type *Get_##name ()const{return this->name;}\
    virtual void Set_##name (type* _arg){this->name = _arg;}

#define QUICK_INSTALL()\
    QuickController controller;\
    Q_UNUSED(controller)\

#define QUICK_INSTALL_DETAILED()\
    QuickApplication::show_detailed_=true;\
    QuickController controller;\
    Q_UNUSED(controller)\


#define QUICK_SETSTYLE(name)\
    QFile file(name);\
    if (file.open(QFile::ReadOnly)) {\
        QString qss = QLatin1String(file.readAll());\
        qApp->setPalette(\
                         QPalette(QColor((qss.indexOf("paletteColor:") >= 0) ?\
                                         qss.mid(qss.indexOf("paletteColor:") +\
                                                 QString("paletteColor:").length(), 7) :\
                                         "#2E2F30")));\
        qApp->setStyleSheet(qss);\
        file.close();\
    }\



#endif // QUICK_H

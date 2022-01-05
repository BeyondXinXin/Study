#include "stable.h"
#include "custom_item.h"
#include <math.h>

// 自定义 Item
MyCustomIQGraphicsEllipseItem::MyCustomIQGraphicsEllipseItem(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent) {
    Initial();
}
void MyCustomIQGraphicsEllipseItem::Initial() {
    QPen p = pen();
    p.setWidth(1);
    p.setColor(QColor(0, 160, 230, 100));
    setPen(p);
    setBrush(QColor(0, 0, 0, 100));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    can_move_ = 1;
}
void MyCustomIQGraphicsEllipseItem::SetThisTitle(QPointF a) {
    this_pointf_ = a ;
}
void MyCustomIQGraphicsEllipseItem::mousePressEvent(
    QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ShiftModifier) {
            setSelected(true);// 选中 item
        } else if (event->modifiers() == Qt::AltModifier) {

        } else {
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    } else if (event->button() == Qt::RightButton) {
        event->ignore();
    }
}


qint32 MyCustomIQGraphicsEllipseItem::type() const {
    return UserType + 1;
}


// 自定义 Item
MyCustomIQGraphicsPixmapItem::MyCustomIQGraphicsPixmapItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent) {
    setZValue(0);
}

MyCustomIQGraphicsPixmapItem::~MyCustomIQGraphicsPixmapItem() {
}

void MyCustomIQGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        thisclecked = 1;
        event->ignore();
    }
    if (event->button() == Qt::RightButton) {
        event->ignore();
    }
}

// 自定义 Item
MyCustomIQGraphicsPathItem::MyCustomIQGraphicsPathItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent) {
    QPen p ;
    p.setWidth(0);
    p.setColor(Qt::red);
    this->setPen(p);
    thisclecked = 0;
    this->setZValue(20);
}

MyCustomIQGraphicsPathItem::~MyCustomIQGraphicsPathItem() {
}

void MyCustomIQGraphicsPathItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        thisclecked = 1;
        event->ignore();
    }
    if (event->button() == Qt::RightButton) {
        event->ignore();
    }
}

// 自定义 Scene
MyCustomQGraphicsScene::MyCustomQGraphicsScene(QGraphicsScene *parent):
    QGraphicsScene(parent) {
    Initial();
}

void MyCustomQGraphicsScene::Initial() {
    middle_.begin = new MyCustomIQGraphicsEllipseItem;
    middle_.end = new MyCustomIQGraphicsEllipseItem;
    addItem(middle_.begin);
    addItem(middle_.end);
    middle_.menu = new QMenu();
    middle_.menu_child = new QMenu();
    middle_.pointnums = 0;
}

void MyCustomQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted()) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::AltModifier) {
            MyCustomIQGraphicsEllipseItem *item =
                new MyCustomIQGraphicsEllipseItem();// 在 Scene 上添加一个自定义 item
            QPen p ;
            p.setWidth(1);
            if (middle_.pointnums == 0) {
                removeItem(middle_.begin);
                p.setColor(QColor(0, 255, 0, 200));
                item->setPen(p);
                QPointF point =  event->scenePos();
                item->SetThisTitle(point);
                item->setRect(point.x() - 2, point.y() - 2, 4, 4);
                middle_.begin = item;
                addItem(item);
                middle_.pointnums++;
            } else if (middle_.pointnums == 1) {
                removeItem(middle_.end);
                p.setColor(QColor(0, 0, 255, 200));
                item->setPen(p);
                QPointF point =  event->scenePos();
                item->SetThisTitle(point);
                item->setRect(point.x() - 2, point.y() - 2, 4, 4);
                middle_.end = item;
                addItem(item);
                middle_.pointnums++;
                emit SignalBtnSeekOut();
            } else {
                QPointF point =  event->scenePos();
                item->SetThisTitle(point);
                item->setRect(point.x() - 2, point.y() - 2, 4, 4);
                addItem(item);
                middle_.list.append(item);
                middle_.pointnums++;
                emit SignalBtnSeekOut();
            }
        }
    }
    if (event->button() == Qt::RightButton) {
        // 检测光标下是否有 item
        MyCustomIQGraphicsEllipseItem *itemthis = nullptr;
        foreach (QGraphicsItem *item, items(event->scenePos())) {
            if (item->type() == QGraphicsItem::UserType + 1) {
                itemthis = static_cast<MyCustomIQGraphicsEllipseItem *>(item) ;
                break;
            }
        }
        if ((itemthis != nullptr) &&
                (itemthis != middle_.begin) &&
                (itemthis != middle_.end)) {
            middle_.menu_child->clear();
            middle_.menu->clear();
            middle_.menu->addAction("删除", [ = ](void) {
                this->SlotDalteOneItem(*itemthis);
            });
            middle_.menu->addAction("取消");
            middle_.menu->addSeparator();
            middle_.menu_child = middle_.menu->addMenu("移动位置");
            for (int i = 1; i <= middle_.list.count(); i++) {
                middle_.menu_child->addAction(QString("移动至%1")
                .arg(i), [ = ](void) {
                    this->SlotChangeSerialItem(*itemthis, i - 1);
                });
            }
            middle_.menu->exec(QCursor::pos());
        }
    }
}



void MyCustomQGraphicsScene::mouseMoveEvent(
    QGraphicsSceneMouseEvent *event) {
    // 检测光标下是否有 item
    MyCustomIQGraphicsEllipseItem *itemthis = nullptr;
    foreach (QGraphicsItem *item, items(event->scenePos())) {
        if (item->type() == QGraphicsItem::UserType + 1) {
            itemthis = static_cast<MyCustomIQGraphicsEllipseItem *>(item) ;
            break;
        }
    }
    if (itemthis != nullptr) {
        emit SignalBtnSeekOut();
    } else {
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void MyCustomQGraphicsScene::SlotDalteOneItem(
    MyCustomIQGraphicsEllipseItem &tem) {
    middle_.list.removeOne(&tem);
    removeItem(&tem);
    MyCustomIQGraphicsEllipseItem *i;
    qint32 tmp_int = 1;
    foreach (i, middle_.list) {
        tmp_int++;
        middle_.pointnums--;
    }
    emit SignalBtnSeekOut();
}

void MyCustomQGraphicsScene::SlotDalteAllItem() {
    MyCustomIQGraphicsEllipseItem *i;
    foreach (i, middle_.list) {
        middle_.list.removeOne(i);
        removeItem(i);
        middle_.pointnums--;
    }
    middle_.pointnums = 0;
    removeItem(middle_.begin);
    removeItem(middle_.end);
    emit SignalBtnSeekOut();
}

void MyCustomQGraphicsScene::SlotChangeSerialItem(
    MyCustomIQGraphicsEllipseItem &tem, int a) {
    middle_.list.removeOne(&tem);
    middle_.list.insert(a, &tem);
    MyCustomIQGraphicsEllipseItem *i;
    qint32 tmp_int = 1;
    foreach (i, middle_.list) {
        tmp_int++;
    }
    emit SignalBtnSeekOut();

}

bool MyCustomQGraphicsScene::SLotGetItemSpoint(
    QList<QPointF> &up_tmp, QList<QPointF> &down_tmp, const QPointF begin) {
    if (middle_.pointnums > 1) {
        middle_.list.insert(0, middle_.begin);
        middle_.list.append(middle_.end);
        MyCustomIQGraphicsEllipseItem *i;
        foreach (i, middle_.list) {
            up_tmp << i->GetThisPosition() + i->pos() - begin;
        }
        middle_.list.removeOne(middle_.begin);
        middle_.list.removeOne(middle_.end);
        return true;
    }
    return false;
}

// 自定义 View
MyCustomQGraphicsView::MyCustomQGraphicsView(QWidget *parent):
    QGraphicsView(parent) {
    this->setStyleSheet("background: transparent;border:0px");
}

void MyCustomQGraphicsView::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0) {
        this->scale(1.1, 1.1);
    } else {
        this->scale(0.9, 0.9);
    }
}

void MyCustomQGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
}





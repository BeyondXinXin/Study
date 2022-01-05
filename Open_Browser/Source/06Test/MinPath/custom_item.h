#ifndef CUSTOM_ITEM_H
#define CUSTOM_ITEM_H

#include "stable.h"

// 自定义 item
class MyCustomIQGraphicsEllipseItem : public QGraphicsEllipseItem {
  public:
    explicit MyCustomIQGraphicsEllipseItem(QGraphicsItem *parent = nullptr);
    void SetThisTitle(QPointF);
    QPointF GetThisPosition() {
        return this_pointf_;
    }
  protected:
    qint32 type() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
  private:
    void Initial();
  private:
    QPointF this_pointf_;
    bool can_move_;

};

// 自定义 item
class MyCustomIQGraphicsPixmapItem : public QGraphicsPixmapItem {
  public:
    explicit MyCustomIQGraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    ~MyCustomIQGraphicsPixmapItem();
    void SetPointF(QList<double> xList, QList<double> yList);
    bool thisclecked;
  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

// 自定义 item
class MyCustomIQGraphicsPathItem : public QGraphicsPathItem {
  public:
    explicit MyCustomIQGraphicsPathItem(QGraphicsItem *parent = nullptr);
    ~MyCustomIQGraphicsPathItem();
    bool thisclecked;
  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

// 自定义 Scene
class MyCustomQGraphicsScene : public QGraphicsScene {
    Q_OBJECT
  public:
    typedef struct {
        MyCustomIQGraphicsEllipseItem *begin;
        MyCustomIQGraphicsEllipseItem *end;
        QList<MyCustomIQGraphicsEllipseItem *> list;
        qint32 pointnums;
        QMenu *menu;
        QMenu *menu_child;
    } line_struct;

    explicit MyCustomQGraphicsScene(QGraphicsScene *parent = nullptr);

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  Q_SIGNALS:
    void SignalBtnSeekOut();
  public Q_SLOTS:
    void SlotDalteOneItem(MyCustomIQGraphicsEllipseItem &tem);
    void SlotDalteAllItem();
    void SlotChangeSerialItem(MyCustomIQGraphicsEllipseItem &tem, int a);
    bool SLotGetItemSpoint(
        QList<QPointF> &point_up, QList<QPointF> &point_down, const QPointF begin);


  private:
    void Initial();
  private:
    line_struct middle_;
};

// 自定义 view
class MyCustomQGraphicsView : public QGraphicsView {
  public:
    explicit MyCustomQGraphicsView(QWidget *parent = nullptr);
  protected:
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CUSTOM_ITEM_H




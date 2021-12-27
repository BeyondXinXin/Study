#include "picture_block_selection_widget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

const static QBrush brush_bg(QColor(0, 0, 0));
const static QBrush brush_select_block(QColor(200, 200, 200, 150));
const static QPen pen_select_frame(Qt::green, 10, Qt::SolidLine);
const static QPen pen_frame(Qt::gray, 5, Qt::SolidLine);
const static QPen pen_name(Qt::red, 10);
const static QPen pen_bg_frame(Qt::gray, 4, Qt::SolidLine);
const static QPoint offset(20, 20);
const static QSize image_size(1024, 1024);

PictureBlockSelectionWidget::PictureBlockSelectionWidget(QWidget *parent)
  : QWidget(parent)
{
    this->Initial();
}

PictureBlockSelectionWidget::~PictureBlockSelectionWidget()
{
}

void PictureBlockSelectionWidget::AddBlock(const QList<QPoint> &path, const QString &name)
{
    block_path_hash_.insert(block_path_hash_.size(), path);
    tag_list_ << name;
}

void PictureBlockSelectionWidget::SetCurrentBlock(const int &id)
{
    if (id >= 0 || id < block_path_hash_.size()) {
        cur_block_ = id;
    }
    this->update();
}

void PictureBlockSelectionWidget::SetHoverBlock(const int &id)
{
    if (id >= 0 || id < block_path_hash_.size()) {
        hover_block_ = id;
    }
    this->update();
}

void PictureBlockSelectionWidget::ClearAllBlock()
{
    tag_list_.clear();
    block_path_hash_.clear();
}

void PictureBlockSelectionWidget::SetBreakGrandImage(const QImage &img)
{
    this->bg_image_ = img.scaled(image_size, Qt::KeepAspectRatio);
}

qreal PictureBlockSelectionWidget::GetImageSizeWidth()
{
    return image_size.width();
}

qreal PictureBlockSelectionWidget::GetImageSizeHeigh()
{
    return image_size.height();
}

void PictureBlockSelectionWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    DrawBg(&painter);
    painter.translate(offset);
    DrawBgImage(&painter);
    DrawFrame(&painter);
    DrawSelectBlock(&painter);
    DrawSelectFrame(&painter);
    DrawBlockName(&painter);
    painter.translate(-offset);
}

void PictureBlockSelectionWidget::mousePressEvent(QMouseEvent *event)
{
    bool contains = false;
    foreach (auto key, block_path_hash_.keys()) {
        QPolygon polygon; //多边形
        foreach (auto var, block_path_hash_.value(key)) {
            polygon << var;
        }
        if (polygon.containsPoint(event->pos() - offset, Qt::OddEvenFill)) {
            cur_block_ = key;
            contains = true;
            break;
        }
    }
    if (!contains) {
        cur_block_ = -1;
    }
    this->update();
    emit SgnCurrentBlockChange(cur_block_);
    QWidget::mousePressEvent(event);
}

void PictureBlockSelectionWidget::mouseMoveEvent(QMouseEvent *event)
{
    bool contains = false;
    foreach (auto key, block_path_hash_.keys()) {
        QPolygon polygon; //多边形
        foreach (auto var, block_path_hash_.value(key)) {
            polygon << var;
        }
        if (polygon.containsPoint(event->pos() - offset, Qt::OddEvenFill)) {
            hover_block_ = key;
            contains = true;
            break;
        }
    }
    if (!contains) {
        hover_block_ = -1;
    }
    this->update();
    emit SgnHoverBlockChange(hover_block_);
    QWidget::mouseMoveEvent(event);
}

void PictureBlockSelectionWidget::Initial()
{
    hover_block_ = -1;
    cur_block_ = -1;
    setMouseTracking(true);
}

void PictureBlockSelectionWidget::DrawBg(QPainter *painter) // 黑色底色和边框
{
    painter->save();
    painter->setBrush(brush_bg);
    painter->drawRect(rect());
    painter->setPen(pen_bg_frame);
    painter->drawRect(rect());
    painter->restore();
}

void PictureBlockSelectionWidget::DrawBgImage(QPainter *painter)
{
    painter->save();
    painter->drawImage(0, 0, bg_image_);
    painter->restore();
}

void PictureBlockSelectionWidget::DrawFrame(QPainter *painter)
{
    painter->save();
    painter->setPen(pen_frame);
    foreach (auto var, block_path_hash_.values()) {
        QPolygon polygon; //多边形
        foreach (auto var, var) {
            polygon << var;
        }
        painter->drawConvexPolygon(polygon);
    }
    painter->restore();
}

void PictureBlockSelectionWidget::DrawSelectFrame(QPainter *painter)
{
    painter->save();
    painter->setPen(pen_select_frame);
    if (block_path_hash_.keys().contains(hover_block_)) {
        QPolygon polygon; //多边形
        foreach (auto var, block_path_hash_.value(hover_block_)) {
            polygon << var;
        }
        painter->drawConvexPolygon(polygon);
    }
    painter->restore();
}

void PictureBlockSelectionWidget::DrawSelectBlock(QPainter *painter)
{
    painter->save();
    painter->setBrush(brush_select_block);
    if (block_path_hash_.keys().contains(cur_block_)) {
        QPolygon polygon; //多边形
        foreach (auto var, block_path_hash_.value(cur_block_)) {
            polygon << var;
        }
        painter->drawPolygon(polygon);
    }
    painter->restore();
}

void PictureBlockSelectionWidget::DrawBlockName(QPainter *painter)
{
    painter->save();
    QFont font;
    font.setPointSize(20);
    painter->setPen(pen_name);
    painter->setFont(font);
    foreach (auto key, block_path_hash_.keys()) {
        painter->drawText(block_path_hash_.value(key).at(0) + QPoint(20, 40), tag_list_.at(key));
    }
    painter->restore();
}

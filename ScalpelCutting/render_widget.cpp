#include "render_widget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>

vtkStandardNewMacro(InteractorStyle);

void InteractorStyle::OnMouseMove()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnMouseMove();
    }
}

void InteractorStyle::OnLeftButtonDown()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
}

void InteractorStyle::OnLeftButtonUp()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
    }
}

void InteractorStyle::OnMiddleButtonDown()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }
}

void InteractorStyle::OnMiddleButtonUp()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
    }
}

void InteractorStyle::OnRightButtonDown()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
}

void InteractorStyle::OnRightButtonUp()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnRightButtonUp();
    }
}

void InteractorStyle::OnMouseWheelForward()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
    }
}

void InteractorStyle::OnMouseWheelBackward()
{
    if (interactive_) {
        vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
    }
}

//------------------------------------------------------
RenderWidget::RenderWidget(QWidget * parent)
  : QVTKOpenGLNativeWidget(parent)
{
    interactor()->SetInteractorStyle(style_);
}

RenderWidget::~RenderWidget()
{
}

void RenderWidget::SetStyleState(const State & state)
{
    switch (state) {
    case DrawCutLine: {
        style_->interactive_ = false;
        state_ = DrawCutLine;
    } break;
    case CutLineInside:
    case CutLineOutside: {
        style_->interactive_ = true;
        state_ = Normal;
        emit SgnCuttingLine(state);
        renderWindow()->Render();
    } break;
    case Normal: {
        style_->interactive_ = true;
        state_ = Normal;

    } break;
    }
    cutting_points_.clear();
}

QPainterPath RenderWidget::GetCuttingPath() const
{
    if (cutting_points_.length() < 1) {
        return QPainterPath();
    }

    QPainterPath path(cutting_points_[0]);
    for (int i = 1; i < cutting_points_.size(); ++i) {
        path.lineTo(cutting_points_[i]);
    }
    return path;
}

void RenderWidget::paintGL()
{
    QVTKOpenGLNativeWidget::paintGL();
    if (state_ == DrawCutLine && !cutting_points_.isEmpty()) {
        QPainter painter(this);
        drawArea(cutting_points_, painter);
    }
}

void RenderWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && state_ == DrawCutLine) {
        leftbtn_drag_ = true;
        cutting_points_.clear();
    }
    QVTKOpenGLNativeWidget::mousePressEvent(event);
}

void RenderWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && state_ == DrawCutLine) {
        leftbtn_drag_ = false;
    }
    QVTKOpenGLNativeWidget::mouseReleaseEvent(event);
}

void RenderWidget::wheelEvent(QWheelEvent * event)
{
    QVTKOpenGLNativeWidget::wheelEvent(event);
}

void RenderWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (leftbtn_drag_) {
        cutting_points_ << QPointF(event->x(), event->y());
        repaint();
    }
    QVTKOpenGLNativeWidget::mouseMoveEvent(event);
}

void RenderWidget::drawArea(QList<QPointF> & pf, QPainter & painter)
{
    if (pf.length() < 1) {
        return;
    }

    QPainterPath path(pf[0]);
    for (int i = 1; i < pf.size(); ++i) {
        path.lineTo(pf[i]);
    }

    QPen pen;
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::green, Qt::Dense4Pattern));
    painter.drawPath(path);
    painter.drawLine(pf[0], pf.last());
}

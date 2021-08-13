#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

class InteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static InteractorStyle *New();
    vtkTypeMacro(InteractorStyle, vtkInteractorStyleTrackballCamera);

    void OnMouseMove() override;
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMiddleButtonDown() override;
    void OnMiddleButtonUp() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;

    bool interactive_ { true };
};

class RenderWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    enum State
    {
        Normal,
        DrawCutLine,
        CutLineInside,
        CutLineOutside,
    };

public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget() override;

    void SetStyleState(const State &state);
    QPolygonF GetCuttingPolygon() const;

signals:
    void SgnCuttingLine(const int &type);

protected:
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void drawArea(QList<QPointF> &pf, QPainter &painter);

private:
    QList<QPointF> cutting_points_ = QList<QPointF>();

    bool leftbtn_drag_ { false };
    State state_ { Normal };

    vtkNew<InteractorStyle> style_;
};

#endif // RENDERWIDGET_H

#include <QApplication>
#include <QPushButton>

#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourValues.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMetaImageReader.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

#include "render_widget.h"

void GenerateTestButton(RenderWidget * wid);

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    // vtk 搭建一个提渲染的Renderer
    vtkNew<vtkMetaImageReader> reader;
    const static QString path = "C:/Users/77935/Pictures/HeadMRVolume/HeadMRVolume.mhd";
    reader->SetFileName(path.toLocal8Bit().data());

    vtkNew<vtkOpenGLGPUVolumeRayCastMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());
    mapper->AutoAdjustSampleDistancesOff();
    mapper->SetSampleDistance(0.5);
    mapper->SetBlendModeToIsoSurface();
    mapper->SetCropping(1);
    mapper->SetCroppingRegionPlanes(50, 150, 50, 200, 50, 150);
    mapper->SetCroppingRegionFlags(VTK_CROP_SUBVOLUME);

    vtkNew<vtkColorTransferFunction> color_transfer_fun;
    color_transfer_fun->RemoveAllPoints();
    vtkNew<vtkNamedColors> colors;
    const auto flesh_color = colors->GetColor3d("flesh").GetData();
    const double iso1 = 40.0;
    color_transfer_fun->AddRGBPoint(iso1, flesh_color[0], flesh_color[1], flesh_color[2]);
    vtkNew<vtkPiecewiseFunction> scalarOpacity;
    scalarOpacity->AddPoint(iso1, 0.6);
    vtkNew<vtkVolumeProperty> volume_property;
    volume_property->ShadeOn();
    volume_property->SetInterpolationTypeToLinear();
    volume_property->SetColor(color_transfer_fun);
    volume_property->SetScalarOpacity(scalarOpacity);
    vtkNew<vtkVolume> volume;
    volume->SetMapper(mapper);
    volume->SetProperty(volume_property);
    vtkNew<vtkRenderer> renderer;
    renderer->AddVolume(volume);
    renderer->SetBackground(colors->GetColor3d("cornflower").GetData());
    renderer->ResetCamera();
    volume_property->GetIsoSurfaceValues()->SetValue(0, iso1);
    renderer->ResetCameraClippingRange();

    // 显示 Wiget
    RenderWidget * wid = new RenderWidget();
    wid->setFixedSize(800, 600);
    wid->renderWindow()->AddRenderer(renderer);
    wid->show();

    // 几个按钮以及逻辑
    GenerateTestButton(wid);

    return a.exec();
}

void GenerateTestButton(RenderWidget * wid)
{
    QPushButton * btn_cut = new QPushButton(wid);
    btn_cut->move(20, 20);
    btn_cut->setFixedSize(60, 30);
    btn_cut->setText("cut");

    QPushButton * btn_cancel = new QPushButton(wid);
    btn_cancel->move(20, 20);
    btn_cancel->setFixedSize(60, 30);
    btn_cancel->setText("cancel");

    QPushButton * btn_inside = new QPushButton(wid);
    btn_inside->move(20, 70);
    btn_inside->setFixedSize(60, 30);
    btn_inside->setText("inside");

    QPushButton * btn_outside = new QPushButton(wid);
    btn_outside->move(20, 120);
    btn_outside->setFixedSize(60, 30);
    btn_outside->setText("outside");

    btn_cut->setVisible(true);
    btn_cancel->setVisible(false);
    btn_inside->setVisible(false);
    btn_outside->setVisible(false);

    static auto FunChangeBtnState = [btn_cut, btn_cancel, btn_inside, btn_outside](const bool & show) {
        btn_cut->setVisible(!show);
        btn_cancel->setVisible(show);
        btn_inside->setVisible(show);
        btn_outside->setVisible(show);
    };

    QObject::connect(btn_cut, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::DrawCutLine);
        FunChangeBtnState(true);
    });

    QObject::connect(btn_cancel, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::Normal);
        FunChangeBtnState(false);
    });

    QObject::connect(btn_inside, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::Normal);
        FunChangeBtnState(false);
    });

    QObject::connect(btn_outside, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::Normal);
        FunChangeBtnState(false);
    });
}

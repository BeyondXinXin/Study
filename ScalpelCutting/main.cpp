﻿#include "render_widget.h"

#include <QApplication>
#include <QDebug>
#include <QPainterPath>
#include <QPushButton>

#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourValues.h>
#include <vtkCoordinate.h>
#include <vtkImageData.h>
#include <vtkImageIterator.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMetaImageReader.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

void Cuting(vtkSmartPointer<vtkImageData> img,
            vtkSmartPointer<vtkVolume> volume,
            vtkSmartPointer<vtkRenderer> renderer,
            const QPainterPath & path, const int & type);
void GenerateTestButton(RenderWidget * wid);

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    // vtk 搭建一个提渲染的Renderer
    vtkNew<vtkMetaImageReader> reader;
    const static QString path = "C:/Users/77935/Pictures/HeadMRVolume/HeadMRVolume.mhd";
    reader->SetFileName(path.toLocal8Bit().data());
    reader->Update();

    auto image_data = vtkSmartPointer<vtkImageData>::New();
    image_data = reader->GetOutput();

    auto mapper = vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper>::New();
    mapper->SetInputData(image_data);
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
    auto volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(mapper);
    volume->SetProperty(volume_property);
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddVolume(volume);
    renderer->SetBackground(colors->GetColor3d("cornflower").GetData());
    renderer->ResetCamera();
    volume_property->GetIsoSurfaceValues()->SetValue(0, iso1);
    renderer->ResetCameraClippingRange();

    // 显示 Wiget
    RenderWidget * wid = new RenderWidget();
    wid->setFixedSize(800, 600);
    wid->renderWindow()->AddRenderer(renderer);

    QObject::connect(wid, &RenderWidget::SgnCuttingLine, wid, [wid, renderer, image_data, volume](const int & type) {
        Cuting(image_data, volume, renderer, wid->GetCuttingPath(), type);
    });

    // 几个按钮以及逻辑
    GenerateTestButton(wid);

    // 显示
    wid->show();
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
        wid->SetStyleState(RenderWidget::CutLineInside);
        FunChangeBtnState(false);
    });

    QObject::connect(btn_inside, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::CutLineOutside);
        FunChangeBtnState(false);
    });

    QObject::connect(btn_outside, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::Normal);
        FunChangeBtnState(false);
    });
}

void Cuting(vtkSmartPointer<vtkImageData> image_data,
            vtkSmartPointer<vtkVolume> volume,
            vtkSmartPointer<vtkRenderer> renderer,
            const QPainterPath & path, const int & type)
{
    int img_dims[3];
    image_data->GetDimensions(img_dims);
    int img_region[] = { 0, img_dims[0] - 1, 0, img_dims[1] - 1, 0, img_dims[2] - 1 };
    vtkImageIterator<unsigned char> img_iterator(image_data, img_region);
    while (!img_iterator.IsAtEnd()) {
        auto iter_begin = img_iterator.BeginSpan();
        auto iter_end = img_iterator.EndSpan();
        for (auto iter = iter_begin; iter != iter_end; ++iter) {
            *iter = 0;
        }
        img_iterator.NextSpan();
    }
    volume->Update();
    renderer->RemoveVolume(volume);
    renderer->AddVolume(volume);
}

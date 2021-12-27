#include "render_widget.h"

#include <QApplication>
#include <QDebug>
#include <QPainterPath>
#include <QPushButton>

#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourValues.h>
#include <vtkCoordinate.h>
#include <vtkImageCast.h>
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

void CutingImagedata(vtkSmartPointer<vtkImageData> img,
                     vtkSmartPointer<vtkVolume> volume,
                     vtkSmartPointer<vtkRenderer> renderer,
                     const QPolygonF &polygon, const int &type);
void GenerateTestButton(RenderWidget *wid);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // vtk 搭建一个提渲染的Renderer
    vtkNew<vtkMetaImageReader> reader;
    const static QString path = "./etc/HeadMRVolume.mhd";
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
    const double iso1 = 100.0;
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
    RenderWidget *wid = new RenderWidget();
    wid->setFixedSize(800, 600);
    wid->renderWindow()->AddRenderer(renderer);

    QObject::connect(wid, &RenderWidget::SgnCuttingLine, wid, [wid, renderer, image_data, volume](const int &type) {
        CutingImagedata(image_data, volume, renderer, wid->GetCuttingPolygon(), type);
    });

    // 几个按钮以及逻辑
    GenerateTestButton(wid);

    // 显示
    wid->show();
    return a.exec();
}

void GenerateTestButton(RenderWidget *wid)
{
    QPushButton *btn_cut = new QPushButton(wid);
    btn_cut->move(20, 20);
    btn_cut->setFixedSize(60, 30);
    btn_cut->setText("cut");

    QPushButton *btn_cancel = new QPushButton(wid);
    btn_cancel->move(20, 20);
    btn_cancel->setFixedSize(60, 30);
    btn_cancel->setText("cancel");

    QPushButton *btn_inside = new QPushButton(wid);
    btn_inside->move(20, 70);
    btn_inside->setFixedSize(60, 30);
    btn_inside->setText("inside");

    QPushButton *btn_outside = new QPushButton(wid);
    btn_outside->move(20, 120);
    btn_outside->setFixedSize(60, 30);
    btn_outside->setText("outside");

    btn_cut->setVisible(true);
    btn_cancel->setVisible(false);
    btn_inside->setVisible(false);
    btn_outside->setVisible(false);

    static auto FunChangeBtnState = [btn_cut, btn_cancel, btn_inside, btn_outside](const bool &show) {
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
        wid->SetStyleState(RenderWidget::CutLineInside);
        FunChangeBtnState(false);
    });

    QObject::connect(btn_outside, &QPushButton::clicked, wid, [&, wid] {
        wid->SetStyleState(RenderWidget::CutLineOutside);
        FunChangeBtnState(false);
    });
}

void CutingImagedata(vtkSmartPointer<vtkImageData> image_data,
                     vtkSmartPointer<vtkVolume> volume,
                     vtkSmartPointer<vtkRenderer> renderer,
                     const QPolygonF &polygon, const int &type)
{
    int img_dims[3];
    double img_spacing[3];
    double img_origian[3];
    image_data->GetDimensions(img_dims);
    image_data->GetSpacing(img_spacing);
    image_data->GetOrigin(img_origian);
    vtkNew<vtkCoordinate> corrdinate;
    corrdinate->SetCoordinateSystemToWorld();
    // 不用vtkImageIterator，迭代器没办法获得空间位置
    // 只是个demo，直接认为vtkImageData 数据是 unsigned char 保存的。如果是其他记得要改。
    // 只是个demo，直接认为bround是从0，0，0开始的，实际工程记得校验。
    // 结果跟狗啃的一样主要是因为模型尺寸比较小（40*40*40），直接抹黑像素肯定很烂
    for (int k = 0; k < img_dims[2]; ++k) {
        for (int i = 0; i < img_dims[0]; ++i) {
            for (int j = 0; j < img_dims[1]; ++j) {
                if (i < 100 && j < 100) {
                    double word_pos[3];
                    word_pos[0] = i * img_spacing[0] + img_origian[0];
                    word_pos[1] = j * img_spacing[1] + img_origian[1];
                    word_pos[2] = k * img_spacing[2] + img_origian[2];
                    corrdinate->SetValue(word_pos);
                    double *display_pos = corrdinate->GetComputedDoubleDisplayValue(renderer);
                    QPointF q_display_pos(display_pos[0], display_pos[1]);
                    if (type == RenderWidget::CutLineInside
                        && polygon.containsPoint(q_display_pos, Qt::OddEvenFill)) {
                        auto pPixel = static_cast<double *>(image_data->GetScalarPointer(i, j, k));
                        *pPixel = 0.0;
                    }
                }
            }
        }
    }
    volume->Update();
    renderer->RemoveVolume(volume);
    renderer->AddVolume(volume);
}

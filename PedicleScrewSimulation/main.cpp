#include "vtk_pediclescrew_source.h"

#include <QApplication>

#include <vtkActor.h>
#include <vtkAutoInit.h>
#include <vtkCubeAxesActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkMultiThreader.h>
#include <vtkOutputWindow.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTextProperty.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

static bool ShowVtkDebugPolydata(vtkSmartPointer<vtkPolyData> surface);

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    vtkNew<VtkPedicleScrewSource> poly_data;
    { // 选填
        poly_data->SetSubdivision(40); // 每一圈螺纹细分数量
        poly_data->SetTopR(3.0); // 圆台顶部半径
        poly_data->SetLength(56.0); // 圆锥高度（不是圆台高度）
        poly_data->SetV(0.06); // 螺旋线前进速度
        poly_data->SetD(0.5); // 螺纹宽度
        poly_data->SetBottonmR(1.0); // 圆台底部半径
        poly_data->SetChamfering(0.3); // 螺纹倒角
        poly_data->SetAscending(0.3); // 螺栓倒角
        poly_data->SetGuideHeight(150); // 引导线高度
        // 模型坐标系乱选的，让他们最后重新提供吧
        poly_data->SetPedicleRotateX(180);
        poly_data->SetPedicleTranslateX(-5);
        poly_data->SetPedicleTranslateY(7);
        poly_data->SetPedicleTranslateZ(18);
    }
    { // 必填
        poly_data->SetPedicleFileName("./etc/Pedicle_screw.STL"); // 椎弓根文件名称
    }
    std::cout << *poly_data << std::endl;
    poly_data->Update();
    ShowVtkDebugPolydata(poly_data->GetOutput());
    return 0;
}

bool ShowVtkDebugPolydata(vtkSmartPointer<vtkPolyData> surface)
{
    vtkNew<vtkPolyDataMapper> polydatamapper;
    polydatamapper->SetInputData(surface);
    vtkNew<vtkActor> actor;
    actor->SetMapper(polydatamapper);
    actor->GetProperty()->SetOpacity(1);
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.2, 0.2, 0.2);
    vtkNew<vtkCubeAxesActor> cubeAxesActor;
    cubeAxesActor->SetCamera(renderer->GetActiveCamera());
    cubeAxesActor->SetBounds(surface->GetBounds());
    cubeAxesActor->SetXTitle("X /mm");
    cubeAxesActor->SetYTitle("Y /mm");
    cubeAxesActor->SetZTitle("Depth /mm");
    cubeAxesActor->GetTitleTextProperty(0)->SetColor(1.0, 0.0, 1.0);
    cubeAxesActor->GetLabelTextProperty(0)->SetColor(1.0, 0.0, 1.0);
    cubeAxesActor->GetTitleTextProperty(1)->SetColor(0.0, 1.0, 0.0);
    cubeAxesActor->GetLabelTextProperty(1)->SetColor(0.0, 1.0, 0.0);
    cubeAxesActor->GetTitleTextProperty(2)->SetColor(0.5, 0.5, 1.0);
    cubeAxesActor->GetLabelTextProperty(2)->SetColor(0.5, 0.5, 1.0);
    cubeAxesActor->SetUseAxisOrigin(1);
    renderer->AddActor(cubeAxesActor);
    vtkNew<vtkRenderWindow> renwin;
    renwin->AddRenderer(renderer);
    renwin->SetSize(800, 800);
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    vtkNew<vtkRenderWindowInteractor> rendererwindowinteracrot;
    rendererwindowinteracrot->SetInteractorStyle(style);
    rendererwindowinteracrot->SetRenderWindow(renwin);
    rendererwindowinteracrot->Start();
    return true;
}

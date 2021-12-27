#include <vtkActor.h>
#include <vtkCubeAxesActor.h>
#include <vtkImageCast.h>
#include <vtkImageData.h>
#include <vtkImageStencil.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPNGWriter.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>

static bool ShowVtkDebugPolydata(vtkSmartPointer<vtkPolyData> surface);
static bool WritePng(const vtkSmartPointer<vtkImageData> & imagedata);

int main(int argc, char * argv[])
{
    vtkNew<vtkPolyData> pd;
    vtkNew<vtkSTLReader> reader;
    if (argc != 2) {
        std::cout << __FUNCDNAME__ << " 使用默认模型模型 ./t.stl " << std::endl;
        reader->SetFileName("./t.stl");
    } else {
        std::cout << __FUNCDNAME__ << " 模型名称： " << argv[1] << std::endl;
        reader->SetFileName(argv[1]);
    }
    reader->Update();
    ShowVtkDebugPolydata(reader->GetOutput());
    pd->DeepCopy(reader->GetOutput());
    vtkNew<vtkImageData> whiteImage;
    double bounds[6];
    pd->GetBounds(bounds);
    double spacing[3];
    spacing[0] = 0.1;
    spacing[1] = 0.1;
    spacing[2] = 1.0;
    whiteImage->SetSpacing(spacing);
    int dim[3];
    for (int i = 0; i < 3; i++) {
        dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]));
    }
    whiteImage->SetDimensions(dim);
    whiteImage->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);
    double origin[3];
    origin[0] = bounds[0] + spacing[0] / 2;
    origin[1] = bounds[2] + spacing[1] / 2;
    origin[2] = bounds[4] + spacing[2] / 2;
    whiteImage->SetOrigin(origin);
    whiteImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
    std::cout << __FUNCDNAME__ << " 输入模型 Bounds： "
              << bounds[0] << " " << bounds[1] << " " << bounds[2] << " "
              << bounds[3] << " " << bounds[4] << " " << bounds[5] << " " << std::endl;
    std::cout << __FUNCDNAME__ << " 输出模型 Spacing "
              << spacing[0] << " " << spacing[1] << " " << spacing[2] << " " << std::endl;
    std::cout << __FUNCDNAME__ << " 输出模型 Dimensions "
              << dim[0] << " " << dim[1] << " " << dim[2] << " " << std::endl;
    std::cout << __FUNCDNAME__ << " 输出模型 Origin "
              << origin[0] << " " << origin[1] << " " << origin[2] << " " << std::endl;
    // 用前景体素填充图像：
    unsigned char inval = 0;
    unsigned char outval = 255;
    vtkIdType count = whiteImage->GetNumberOfPoints();
    for (vtkIdType i = 0; i < count; ++i) {
        whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
    }
    // 多边形数据->图像模具：
    vtkNew<vtkPolyDataToImageStencil> pol2stenc;
    pol2stenc->SetInputData(pd);
    pol2stenc->SetOutputOrigin(origin);
    pol2stenc->SetOutputSpacing(spacing);
    pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
    // 剪切相应的白色图像并设置背景：
    vtkNew<vtkImageStencil> imgstenc;
    imgstenc->SetInputData(whiteImage);
    imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
    imgstenc->ReverseStencilOff();
    imgstenc->SetBackgroundValue(outval);
    imgstenc->Update();
    WritePng(imgstenc->GetOutput());
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

#include <direct.h>
#include <io.h>

bool WritePng(const vtkSmartPointer<vtkImageData> & imagedata)
{
    std::string folder_path = "./out";
    system("rd /s/q out");
    _mkdir(folder_path.c_str());
    vtkNew<vtkImageCast> cast_filter;
    cast_filter->SetOutputScalarTypeToUnsignedChar();
    cast_filter->SetInputData(imagedata);
    vtkNew<vtkPNGWriter> writer;
    writer->SetFilePattern("./out/%d.png");
    writer->SetFileDimensionality(3);
    writer->SetInputConnection(cast_filter->GetOutputPort());
    writer->Write();
    return true;
}

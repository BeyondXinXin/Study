#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkIdFilter.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkSphereSource.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

static QStringList filename {
    "./etc/STL/L5.STL",

    "./etc/STL/S1.STL",
    "./etc/STL/S2.STL",
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->SetSize(640, 480);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("HighlightPicked");
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);
    vtkNew<vtkInteractorStyleTrackballActor> style;
    style->SetDefaultRenderer(renderer);
    renderWindowInteractor->SetInteractorStyle(style);
    foreach (auto var, filename) {
        vtkNew<vtkSTLReader> reader;
        reader->SetFileName(var.toLocal8Bit().data());
        vtkNew<vtkSphereSource> source;
        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(reader->GetOutputPort());
        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        actor->GetProperty()->SetSpecularColor(colors->GetColor3d("White").GetData());
        actor->GetProperty()->SetSpecularPower(30.0);
        renderer->AddActor(actor);
    }
    renderWindow->Render();
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();

    return 0;
}

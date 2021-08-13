#include <vtkActor.h>
#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTexture.h>
#include <vtkTexturedSphereSource.h>
#include <vtkTransformTextureCoords.h>

int main(int, char *[])
{

    vtkNew<vtkTexturedSphereSource> sphere;
    sphere->SetPhiResolution(32);
    sphere->SetRadius(10 * 6.371e6 / 6371 * 10e3);
    sphere->SetThetaResolution(32);
    vtkNew<vtkImageReader2Factory> reader_factory;
    vtkImageReader2 *image_reader = reader_factory->CreateImageReader2("./etc/earth.jpg");
    image_reader->SetFileName("./etc/earth.jpg");
    vtkNew<vtkTexture> texture;
    texture->SetInputConnection(image_reader->GetOutputPort());

    vtkNew<vtkTransformTextureCoords> transformTexture;
    transformTexture->SetInputConnection(sphere->GetOutputPort());
    transformTexture->SetPosition(0, 0, 0);
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(transformTexture->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->SetTexture(texture);

    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> render_window;
    render_window->AddRenderer(renderer);

    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(render_window);

    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.1);

    render_window->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}

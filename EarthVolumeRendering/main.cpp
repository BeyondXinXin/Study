#include <direct.h>
#include <io.h>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkCubeAxesActor.h>
#include <vtkDataArray.h>
#include <vtkImageCast.h>
#include <vtkImageData.h>
#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageShiftScale.h>
#include <vtkImageStencil.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLookupTable.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPNGWriter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkSampleFunction.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkSphere.h>
#include <vtkSpheres.h>
#include <vtkSuperquadric.h>
#include <vtkTextProperty.h>
#include <vtkVolumeProperty.h>
#include <vtkXMLImageDataReader.h>

#include <QDebug>

void CreateImageData(vtkImageData *im);

int main(int, char *[])
{
    vtkNew<vtkImageData> image_data;
    CreateImageData(image_data);

    vtkNew<vtkRenderWindow> ren_win;
    vtkNew<vtkRenderer> render;
    render->SetBackground(0.1, 0.1, 0.1);
    ren_win->AddRenderer(render);
    ren_win->SetSize(301, 300);

    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(ren_win);
    ren_win->Render();

    vtkNew<vtkSmartVolumeMapper> volume_mapper;
    volume_mapper->SetBlendModeToComposite();
    volume_mapper->SetInputData(image_data);

    vtkNew<vtkVolumeProperty> volume_property;

    vtkNew<vtkPiecewiseFunction> composite_opacity;
    composite_opacity->AddPoint(499, 0);
    composite_opacity->AddPoint(500.0, 1);
    composite_opacity->AddPoint(2500, 1);
    composite_opacity->AddPoint(2501, 0);

    volume_property->SetScalarOpacity(composite_opacity);

    vtkNew<vtkColorTransferFunction> color;
    color->AddRGBPoint(0.0, 0.0, 0.0, 0.0);

    color->AddRGBPoint(500.0, 2.0 / 255.0, 5.0 / 255.0, 20.0 / 255.0);
    color->AddRGBPoint(550.0, 5.0 / 255.0, 12.0 / 255.0, 30.0 / 255.0);
    color->AddRGBPoint(600.0, 13.0 / 255.0, 38.0 / 255.0, 76.0 / 255.0);
    color->AddRGBPoint(650.0, 20.0 / 255.0, 49.0 / 255.0, 93.0 / 255.0);
    color->AddRGBPoint(700.0, 25.0 / 255.0, 63.0 / 255.0, 110.0 / 255.0);
    color->AddRGBPoint(800.0, 31.0 / 255.0, 75.0 / 255.0, 124.0 / 255.0);
    color->AddRGBPoint(900.0, 23.0 / 255.0, 36.0 / 255.0, 8.0 / 255.0);
    color->AddRGBPoint(1000.0, 50.0 / 255.0, 58.0 / 255.0, 21.0 / 255.0);
    color->AddRGBPoint(1200.0, 99.0 / 255.0, 86.0 / 255.0, 54.0 / 255.0);
    color->AddRGBPoint(1400.0, 165.0 / 255.0, 150.0 / 255.0, 117.0 / 255.0);
    color->AddRGBPoint(2500.0, 249.0 / 255.0, 251.0 / 255.0, 255.0 / 255.0);

    volume_property->SetColor(color);

    vtkNew<vtkVolume> volume;
    volume->SetMapper(volume_mapper);
    volume->SetProperty(volume_property);
    render->AddViewProp(volume);
    render->ResetCamera();

    ren_win->Render();
    volume_mapper->SetRequestedRenderModeToRayCast();
    ren_win->Render();
    iren->Start();

    return EXIT_SUCCESS;
}

class vtkEarthSphere : public vtkImplicitFunction
{
public:
    vtkTypeMacro(vtkEarthSphere, vtkImplicitFunction);
    void PrintSelf(ostream &os, vtkIndent indent) override;
    static vtkEarthSphere *New();
    using vtkImplicitFunction::EvaluateFunction;
    double EvaluateFunction(double pos[3]) override;
    void EvaluateGradient(double x[3], double n[3]) override;

protected:
    vtkEarthSphere()
    {
        vtkNew<vtkImageReader2Factory> reader_factory;
        auto image_reader = reader_factory->CreateImageReader2("./etc/gebco.png");
        image_reader->SetFileName("./etc/gebco.png");
        image_reader->Update();
        trexture_->DeepCopy(image_reader->GetOutput());
        image_reader->Delete();
    }
    ~vtkEarthSphere() override
    {
    }

private:
    vtkEarthSphere(const vtkEarthSphere &) = delete;
    void operator=(const vtkEarthSphere &) = delete;

    vtkNew<vtkImageData> trexture_;
};

vtkStandardNewMacro(vtkEarthSphere);
void vtkEarthSphere::PrintSelf(std::ostream &os, vtkIndent indent)
{
    Q_UNUSED(os)
    Q_UNUSED(indent)
}

double vtkEarthSphere::EvaluateFunction(double pos[3])
{
    const int x_dim = trexture_->GetDimensions()[0] - 1;
    const int y_dim = trexture_->GetDimensions()[1] - 1;

    double tc[2];
    double pixel = 0.0, phi = 0.0, thetaX = 0.0, thetaY = 0.0;
    const auto x = pos[0];
    const auto y = pos[1];
    const auto z = pos[2];
    const auto r = sqrt(x * x + y * y + z * z);
    if (abs(r - .8) < 0.02) {
        if (z > r) {
            phi = 0.0;
            if (z > 0.0) {
                tc[1] = 0.0;
            } else {
                tc[1] = 1.0;
            }
        } else {
            phi = acos(z / r);
            tc[1] = phi / vtkMath::Pi();
        }
        const auto rho = r * sin(phi);
        if (rho != 0.0) {
            if (abs(x) > rho) {
                if (x > 0.0) {
                    thetaX = 0.0;
                } else {
                    thetaX = vtkMath::Pi();
                }
            } else {
                thetaX = acos(x / rho);
            }
            if (abs(y) > rho) {
                if (y > 0.0) {
                    thetaY = vtkMath::Pi() / 2.0;
                } else {
                    thetaY = -vtkMath::Pi() / 2.0;
                }
            } else {
                thetaY = asin(y / rho);
            }
        } else {
            thetaX = thetaY = 0.0;
        }
        tc[0] = thetaX / (2.0 * vtkMath::Pi());
        if (thetaY < 0.0) {
            tc[0] = 1.0 - tc[0];
        }
        const auto trexture_x = static_cast<int>(tc[0] * x_dim);
        const auto trexture_y = y_dim - static_cast<int>(tc[1] * y_dim);
        unsigned char *gray_pixel = static_cast<unsigned char *>(
          trexture_->GetScalarPointer(trexture_x, trexture_y, 0));

        pixel = 550.0 + 2000.0 * (static_cast<double>(*gray_pixel) / 240.0);

    } else {
        pixel = -1000;
    }

    return pixel;
}

void vtkEarthSphere::EvaluateGradient(double x[3], double n[3])
{
    Q_UNUSED(x)
    Q_UNUSED(n)
}

void CreateImageData(vtkImageData *imageData)
{
    vtkNew<vtkEarthSphere> sphere;
    vtkNew<vtkSampleFunction> sampleFunction;
    sampleFunction->SetImplicitFunction(sphere);
    sampleFunction->SetSampleDimensions(255, 255, 255);
    sampleFunction->SetModelBounds(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    sampleFunction->Update();
    imageData->ShallowCopy(sampleFunction->GetOutput());
}

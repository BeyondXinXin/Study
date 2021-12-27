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
#include <vtkInteractorStyleTrackballCamera.h>
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

static QStringList filename {
    "./etc/STL/C1.STL",
    "./etc/STL/C2.STL",
    "./etc/STL/C3.STL",
    "./etc/STL/C4.STL",
    "./etc/STL/C5.STL ",
    "./etc/STL/C6.STL",
    "./etc/STL/C7.STL",

    "./etc/STL/T1.STL",
    "./etc/STL/T2.STL",
    "./etc/STL/T3.STL",
    "./etc/STL/T4.STL",
    "./etc/STL/T5.STL",
    "./etc/STL/T6.STL",
    "./etc/STL/T7.STL",
    "./etc/STL/T8.STL",
    "./etc/STL/T9.STL",
    "./etc/STL/T10.STL",
    "./etc/STL/T11.STL",
    "./etc/STL/T12.STL",

    "./etc/STL/L1.STL",
    "./etc/STL/L2.STL",
    "./etc/STL/L3.STL",
    "./etc/STL/L4.STL",
    "./etc/STL/L5.STL",

    "./etc/STL/S1.STL",
    "./etc/STL/S2.STL",
};

class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
{
public:
    vtkTypeMacro(MouseInteractorHighLightActor, vtkInteractorStyleTrackballCamera);
    static MouseInteractorHighLightActor * New();
    MouseInteractorHighLightActor();
    ~MouseInteractorHighLightActor() override;
    void OnLeftButtonDown() override;
    void OnMouseMove() override;
    void Rotate() override;
    QList<vtkActor *> actors_;

private:
    vtkActor * LastPickedActor;
    vtkProperty * LastPickedProperty;
};

MouseInteractorHighLightActor::MouseInteractorHighLightActor()
{
    LastPickedActor = nullptr;
    LastPickedProperty = vtkProperty::New();
}

MouseInteractorHighLightActor::~MouseInteractorHighLightActor()
{
    LastPickedProperty->Delete();
}

void MouseInteractorHighLightActor::OnLeftButtonDown()
{
    vtkNew<vtkNamedColors> colors;
    int * clickPos = GetInteractor()->GetEventPosition();
    vtkNew<vtkPropPicker> picker;
    picker->Pick(clickPos[0], clickPos[1], 0, GetDefaultRenderer());
    if (LastPickedActor) {
        LastPickedActor->GetProperty()->DeepCopy(LastPickedProperty);
    }
    LastPickedActor = picker->GetActor();
    if (LastPickedActor) {
        LastPickedProperty->DeepCopy(LastPickedActor->GetProperty());
        LastPickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
        LastPickedActor->GetProperty()->SetOpacity(1.0);
    }
    int i = 0;
    bool select = false;
    foreach (auto actor, actors_) {
        if (actor == LastPickedActor) {
            select = true;
            InvokeEvent(vtkCommand::UserEvent + 1, &i);
        }
        i++;
    }
    if (!select) {
        i = -1;
        InvokeEvent(vtkCommand::UserEvent + 1, &i);
    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void MouseInteractorHighLightActor::OnMouseMove()
{
    int x = Interactor->GetEventPosition()[0];
    int y = Interactor->GetEventPosition()[1];
    switch (State) {
    case VTKIS_ROTATE:
        FindPokedRenderer(x, y);
        Rotate();
        InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;
    case VTKIS_PAN:
        FindPokedRenderer(x, y);
        Pan();
        InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;
    }
}

void MouseInteractorHighLightActor::Rotate()
{
    if (CurrentRenderer == nullptr) {
        return;
    }
    vtkRenderWindowInteractor * rwi = Interactor;
    int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
    int dy = 0;
    int * size = CurrentRenderer->GetRenderWindow()->GetSize();
    double delta_elevation = -20.0 / size[1];
    double delta_azimuth = -20.0 / size[0];
    double rxf = dx * delta_azimuth * MotionFactor;
    double ryf = dy * delta_elevation * MotionFactor;
    vtkCamera * camera = CurrentRenderer->GetActiveCamera();
    camera->Azimuth(rxf);
    camera->Elevation(ryf);
    camera->OrthogonalizeViewUp();

    if (AutoAdjustCameraClippingRange) {
        CurrentRenderer->ResetCameraClippingRange();
    }

    if (rwi->GetLightFollowCamera()) {
        CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    rwi->Render();
}

vtkStandardNewMacro(MouseInteractorHighLightActor);

class SelkectCallback : public vtkCommand
{
public:
    static SelkectCallback * New()
    {
        return new SelkectCallback;
    }
    virtual void Execute(vtkObject * caller, unsigned long role, void * data)
    {
        if (role == vtkCommand::UserEvent + 1) {
            const auto select_id = static_cast<int *>(data);
            lab->setText(QString("Select:   %1  ").arg(*select_id));
        }
    }

    QLabel * lab;
};

int main(int argc, char * argv[])
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
    vtkNew<MouseInteractorHighLightActor> style;
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
        style->actors_ << actor;
    }

    QWidget widget;
    QVBoxLayout * layout = new QVBoxLayout(); //水平布局
    QLabel * lab = new QLabel(&widget);
    layout->addWidget(lab);
    int obj_id = 0;
    foreach (auto var, filename) {
        QCheckBox * box = new QCheckBox(&widget);
        box->setText(var);
        box->setObjectName(QString("%1").arg(obj_id));
        box->setChecked(true);
        layout->addWidget(box);
        QObject::connect(
          box, &QCheckBox::stateChanged, &widget,
          [obj_id, &style, &renderer, &renderWindow, &renderWindowInteractor](int state) {
              if (state == Qt::Unchecked) {
                  style->actors_[obj_id]->SetVisibility(false);
              } else if (state == Qt::Checked) {
                  style->actors_[obj_id]->SetVisibility(true);
              }
              renderWindowInteractor->Render();
              renderer->ResetCamera();
          });
        obj_id++;
    }
    widget.setLayout(layout);
    widget.setFixedSize(400, 900);
    widget.show();

    vtkNew<SelkectCallback> callback;
    callback->lab = lab;
    style->AddObserver(vtkCommand::UserEvent + 1, callback);

    renderWindow->Render();
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();

    return 0;
}

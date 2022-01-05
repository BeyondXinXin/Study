#include "formedgeadjustment.h"
#include "ui_formedgeadjustment.h"

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>


#include <vtkRenderWindow.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkAxisActor2D.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkLegendScaleActor.h>
#include <vtkImageActor.h>
#include <vtkImageSlice.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageProperty.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPNGWriter.h>
#include <vtkCommand.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageReader2Factory.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkSeedRepresentation.h>
#include <vtkKdTreePointLocator.h>
#include <vtkDijkstraImageContourLineInterpolator.h>
#include <vtkDijkstraImageGeodesicPath.h>
#include <vtkProperty.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkContourTriangulator.h>
#include <vtkDataSetMapper.h>
#include <vtkHandleWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkContextScene.h>
#include <vtkContextActor.h>
#include <vtkPlot.h>
#include <vtkAxis.h>
#include <vtkPen.h>
#include <vtkNamedColors.h>
#include <vtkLineSource.h>
#include <vtkGlyph3DMapper.h>
#include <vtkAbstractPicker.h>
#include <vtkImageReader2.h>
#include <vtkInteractorStyleImage.h>




FormEdgeAdjustment::FormEdgeAdjustment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdgeAdjustment) {
    ui->setupUi(this);
    this->setWindowTitle("边缘调整控件测试");
    ui->widget->setFixedSize(1000, 1000);

    //
    line_adjustment_size_ = 10;
    current_adjust_bool_ << 0 << 0 << 0;
    number_adjust_left_ = 0;
    number_adjust_regit_ = 0;
    number_currentl_left_ = 0;
    number_currentl_regit_ = 0;
    adjust_click_ = {0, 0, 0, 0};

    // 窗口初始化
    select_render_ = vtkSmartPointer<vtkRenderer>::New();
    select_render_->SetBackground(90 / 255.0, 90 / 255.0, 90 / 255.0);
    this->render_window_ = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->SetRenderWindow(this->render_window_);
    vtkNew<vtkInteractorStyleImage> vtk_select_interactor_style;
    ui->widget->GetRenderWindow()->GetInteractor()
    ->SetInteractorStyle(vtk_select_interactor_style);
    this->render_window_->AddRenderer(this->select_render_);
    this->render_window_->SetPointSmoothing(true);
    this->render_window_->SetLineSmoothing(true);
    this->render_window_->SetPolygonSmoothing(false);
    this->select_render_->Render();

    // 打开图片
    QString image_path = "minpath.png";
    vtkNew<vtkImageReader2Factory> reader_factory;
    vtkSmartPointer<vtkImageReader2> image_reader =
        reader_factory->CreateImageReader2(image_path.toLocal8Bit().data());
    image_reader->SetFileName(image_path.toLocal8Bit().data());
    image_reader->Update();
    vtkNew<vtkImageActor> image_actor;
    image_actor->SetInputData(image_reader->GetOutput());
    select_render_->AddActor(image_actor);
    double origin[3];
    double spacing[3];
    int extent[6];
    image_reader->GetOutput()->GetOrigin(origin);
    image_reader->GetOutput()->GetSpacing(spacing);
    image_reader->GetOutput()->GetExtent(extent);
    vtkSmartPointer<vtkCamera> camera = select_render_->GetActiveCamera();
    camera->ParallelProjectionOn();
    double xc = origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0];
    double yc = origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1];
    double xd = (extent[1] - extent[0] + 1) * spacing[0];
    double yd = (extent[3] - extent[2] + 1) * spacing[1];
    double d = camera->GetDistance();
    Q_UNUSED(xd)
    camera->SetFocalPoint(xc, yc, 0.0);
    camera->SetPosition(xc, yc, d);
    camera->SetParallelScale(0.5 * (yd - 1));

    SetAdjustWidget(1, 1);
}

FormEdgeAdjustment::~FormEdgeAdjustment() {
    delete ui;
}

void FormEdgeAdjustment::SlotAdjustClickBegin() {
    ui->widget->GetInteractor()->GetPicker()->Pick(
        ui->widget->GetInteractor()->GetEventPosition()[0],
        ui->widget->GetInteractor()->GetEventPosition()[1],
        0, select_render_);
    double picked[3];
    ui->widget->GetInteractor()->GetPicker()->GetPickPosition(picked);
    if (picked[0] > 0 && picked[1] > 0) {
        edge_chinge_id_.clear();
        SetCursorChange();
        this->current_adjust_bool_[0] = 1;

        for (qint32 i = 0; i < number_adjust_left_ + number_adjust_regit_ ; ++i) {
            double x_difference = points_->GetPoint(i)[0] - picked[0];
            double y_difference = points_->GetPoint(i)[1] - picked[1];
            if (qAbs(x_difference) < line_adjustment_size_) {
                if (qAbs(y_difference) < line_adjustment_size_) {
                    edge_chinge_id_ << i;
                }
            }
        }
        qSort(edge_chinge_id_.begin(), edge_chinge_id_.end());
    }
    select_render_->Render();
}

void FormEdgeAdjustment::SlotAdjustClickEnd() {
    if (current_adjust_bool_[1]) {
        QList<QList<double>> points_l;
        for (qint32 i = 0; i < number_adjust_left_; i++) {
            points_l.append({points_->GetPoint(i)[0], points_->GetPoint(i)[1], 0});
        }
        splinefiltering(points_l,
                        edge_chinge_id_.at(0),
                        number_currentl_left_ + edge_chinge_id_.at(0));
        for (qint32 i = 0; i < number_adjust_left_; i++) {
            points_->InsertPoint(
                static_cast<vtkIdType>(i),
                points_l.at(i)[0], points_l.at(i)[1], points_l.at(i)[2]);
        }
    }
    if (current_adjust_bool_[2]) {
        QList<QList<double>> points_r;
        for (qint32 i = number_adjust_left_;
                i < number_adjust_left_ + number_adjust_regit_; i++) {
            points_r.append({points_->GetPoint(i)[0], points_->GetPoint(i)[1], 0});
        }
        splinefiltering(
            points_r,
            edge_chinge_id_.at(number_currentl_left_) - number_adjust_left_,
            number_currentl_regit_ + number_currentl_left_
            - number_adjust_left_ + edge_chinge_id_.at(0));
        for (qint32 i = 0; i <  number_adjust_regit_; i++) {
            points_->InsertPoint(
                static_cast<vtkIdType>(i + number_adjust_left_),
                points_r.at(i)[0], points_r.at(i)[1], points_r.at(i)[2]);
        }
    }
    if (current_adjust_bool_[1] || current_adjust_bool_[2]) {
        vtkNew<vtkPolyDataMapper> mapper ;
        mapper->SetInputData(polydata_adjust_);
        adjust_actor_list_.at(adjust_actor_list_.count() - 1)->SetMapper(mapper);
        vtkNew<vtkGlyph3DMapper> splinePointsMapper ;
        splinePointsMapper->SetInputData(polydata_adjust_);
        splinePointsMapper->SetSourceConnection(sphereSource->GetOutputPort());
        adjust_actor_list_.at(adjust_actor_list_.count() - 2)
        ->SetMapper(splinePointsMapper);
        ui->widget->GetRenderWindow()->Render();
    }
    ui->widget->setCursor(QCursor(Qt::ArrowCursor));
    current_adjust_bool_[0] = 0;
    current_adjust_bool_[1] = 0;
    current_adjust_bool_[2] = 0;
}

void FormEdgeAdjustment::SlotAdjustMove() {
    adjust_click_[0] = ui->widget->GetInteractor()->GetEventPosition()[0];
    adjust_click_[1] =  ui->widget->GetInteractor()->GetEventPosition()[1];
    if (1 ==  this->current_adjust_bool_[0]) {
        double x = adjust_click_[2] - adjust_click_[0];
        double y = adjust_click_[3] - adjust_click_[1];
        if (0.0 == x && 0.0 == y) {
            return;
        }
        double scale = select_render_->GetActiveCamera()->GetParallelScale() / 500;
        x *= scale / 0.8;
        y *= scale / 0.8;
        number_currentl_left_ = 0;
        number_currentl_regit_ = 0;
        for (qint32 i = 0; i < edge_chinge_id_.count(); ++i) {
            qint32 tmp = edge_chinge_id_.at(i);
            if (tmp < number_adjust_left_) {
                number_currentl_left_ = i + 1;
            }
        }
        number_currentl_regit_ = edge_chinge_id_.count() - number_currentl_left_;
        for (qint32 i = 0; i < number_currentl_left_; ++i) {
            qint32 tmp = edge_chinge_id_.at(0) + i ;
            double tmp_split = ((i + 1.0)  * (number_currentl_left_  - i) * 2) /
                               (number_currentl_left_ * number_currentl_left_);
            tmp_split = tmp_split > 0.5 ? 0.5 : tmp_split;
            double tmp_x = tmp_split  * tmp_split * x;
            double tmp_y = tmp_split  * tmp_split * y;
            double x_different = (points_->GetPoint(tmp)[0] - tmp_x);
            double y_different = (points_->GetPoint(tmp)[1] - tmp_y);
            points_->SetPoint(tmp, x_different, y_different, 0);
            current_adjust_bool_[1] = 1;
        }
        for (qint32 i = 0; i < number_currentl_regit_; ++i) {
            qint32 tmp = edge_chinge_id_.at(number_currentl_left_) + i ;
            double tmp_split = ((i + 1.0) * (number_currentl_regit_  - i) * 2) /
                               (number_currentl_regit_ * number_currentl_regit_);
            tmp_split = tmp_split > 0.5 ? 0.5 : tmp_split;
            double tmp_x = tmp_split  * tmp_split * x;
            double tmp_y = tmp_split  * tmp_split * y;
            double x_different = (points_->GetPoint(tmp)[0] - tmp_x);
            double y_different = (points_->GetPoint(tmp)[1] - tmp_y);
            points_->SetPoint(tmp, x_different, y_different, 0);
            current_adjust_bool_[2] = 1;
        }
        vtkNew<vtkPolyDataMapper> mapper ;
        mapper->SetInputData(polydata_adjust_);
        adjust_actor_list_.at(adjust_actor_list_.count() - 1)->SetMapper(mapper);
        vtkNew<vtkGlyph3DMapper> splinePointsMapper ;
        splinePointsMapper->SetInputData(polydata_adjust_);
        splinePointsMapper->SetSourceConnection(sphereSource->GetOutputPort());
        adjust_actor_list_.at(adjust_actor_list_.count() - 2)
        ->SetMapper(splinePointsMapper);
        ui->widget->GetRenderWindow()->Render();
    }
    adjust_click_[2] = adjust_click_[0];
    adjust_click_[3] = adjust_click_[1];
}

void FormEdgeAdjustment::SlotAdjustKeyPress() {
    QString key = ui->widget->GetRenderWindow()->GetInteractor()
                  ->GetInteractorStyle()->GetInteractor()->GetKeySym();
    if (0 == key.compare("Down")) {
        if (line_adjustment_size_ > 1) {
            --line_adjustment_size_;
        }
        SetCursorChange();
    } else if (0 == key.compare("Up")) {
        if (line_adjustment_size_ < 20) {
            ++line_adjustment_size_;
        }
        SetCursorChange();
    }
}

void FormEdgeAdjustment::SlotAdjustKeyRelese() {
    QString key = ui->widget->GetRenderWindow()->GetInteractor()
                  ->GetInteractorStyle()->GetInteractor()->GetKeySym();
    if (0 == key.compare("Down")) {
        ui->widget->setCursor(QCursor(Qt::ArrowCursor));
    } else if (0 == key.compare("Up")) {
        ui->widget->setCursor(QCursor(Qt::ArrowCursor));
    }
}


void FormEdgeAdjustment::SetAdjustWidget(const bool adjust_l, const bool adjust_r) {
    points_ = vtkSmartPointer<vtkPoints>::New();
    QList<QList<double>> points_l, points_r;
    ifstream pathdata_left;
    ifstream pathdata_right;
    pathdata_left.open("Leftpoints.dat", ios::in);
    pathdata_right.open("Rightpoints.dat", ios::in);
    pathdata_left >> number_adjust_left_;
    pathdata_right >> number_adjust_regit_;
    if (0 == number_adjust_left_) {
        return;
    }
    double pointsX, pointsY, pointsZ;
    for (qint32 i = 0; i < number_adjust_left_; i++) {
        pathdata_left >> pointsX >> pointsY >> pointsZ;
        points_l.append({pointsX, pointsY, pointsZ});
    }
    for (qint32 i = number_adjust_left_;
            i < number_adjust_left_ + number_adjust_regit_; i++) {
        pathdata_right >> pointsX >> pointsY >> pointsZ;
        points_r.append({pointsX, pointsY, pointsZ});
    }
    pathdata_left.close();
    pathdata_right.close();

    if (adjust_l) {
        filtering(points_l);
    }
    if (adjust_r) {
        filtering(points_r);
    }

    for (qint32 i = 0; i < number_adjust_left_; i++) {
        points_->InsertPoint(static_cast<vtkIdType>(i),
                             points_l.at(i)[0], points_l.at(i)[1], points_l.at(i)[2]);
    }
    for (qint32 i = 0; i <  number_adjust_regit_; i++) {
        points_->InsertPoint(static_cast<vtkIdType>(i + number_adjust_left_),
                             points_r.at(i)[0], points_r.at(i)[1], points_r.at(i)[2]);
    }
    vtkIdType *vertexIndices_left_ ;// 左侧边缘点id
    vtkIdType *vertexIndices_right_ ;// 右侧边缘点id
    // 创建单元阵列以将点连接到有意义的几何图形中
    vertexIndices_left_ =
        new vtkIdType[static_cast<unsigned long>(number_adjust_left_ + 1)];
    vertexIndices_right_ =
        new vtkIdType[static_cast<unsigned long>(number_adjust_regit_ + 1)];
    for (qint32 i = 0; i < number_adjust_left_; i++) {
        vertexIndices_left_[i] = static_cast<vtkIdType>(i);
    }
    for (qint32 i = 0; i < number_adjust_regit_; i++) {
        vertexIndices_right_[i] = static_cast<vtkIdType>(number_adjust_left_ + i);
    }
    vtkNew<vtkCellArray> lines;
    lines->InsertNextCell(number_adjust_left_, vertexIndices_left_);
    lines->InsertNextCell(number_adjust_regit_, vertexIndices_right_);
    polydata_adjust_->SetPoints(points_);
    polydata_adjust_->SetLines(lines);
    vtkNew<vtkActor> PolyActor;
    vtkNew<vtkActor> pointsActor;
    vtkNew<vtkPolyDataMapper> mapper ;
    mapper->SetInputDataObject(polydata_adjust_);
    PolyActor->SetMapper(mapper);
    PolyActor-> PickableOff();
    sphereSource->SetPhiResolution(21);
    sphereSource->SetThetaResolution(21);
    sphereSource->SetRadius(.1);
    vtkNew<vtkGlyph3DMapper> splinePointsMapper ;
    this->vtk_connections_ = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    splinePointsMapper->SetInputData(polydata_adjust_);
    splinePointsMapper->SetSourceConnection(sphereSource->GetOutputPort());
    pointsActor->SetMapper(splinePointsMapper);
    adjust_actor_list_.clear();
    adjust_actor_list_.append(PolyActor);
    adjust_actor_list_.append(pointsActor);
    select_render_->AddActor(PolyActor);
    select_render_->AddActor(pointsActor);

    this->vtk_connections_ = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    vtk_connections_->Connect(
        ui->widget->GetInteractor()->GetInteractorStyle(),
        vtkCommand::LeftButtonPressEvent,
        this, SLOT(SlotAdjustClickBegin()));
    vtk_connections_->Connect(
        ui->widget->GetInteractor()->GetInteractorStyle(),
        vtkCommand::LeftButtonReleaseEvent,
        this, SLOT(SlotAdjustClickEnd()));
    vtk_connections_->Connect(
        ui->widget->GetInteractor()->GetInteractorStyle(),
        vtkCommand::KeyPressEvent,
        this, SLOT(SlotAdjustKeyPress()));
    vtk_connections_->Connect(
        ui->widget->GetInteractor()->GetInteractorStyle(),
        vtkCommand::KeyReleaseEvent,
        this, SLOT(SlotAdjustKeyRelese()));
    vtk_connections_->Connect(
        ui->widget->GetInteractor(),
        vtkCommand::MouseMoveEvent,
        this, SLOT(SlotAdjustMove()));
    ui->widget->GetRenderWindow()->Render();
}


void FormEdgeAdjustment::filtering(QList<QList<double> > &data) {
    qint32 a1, a2, a3, total, N;
    a1 = 1;
    a2 = 1;
    a3 = 1;
    total = a1 + a2 + a3;
    N = data.size();
    QList<QList<double>> data_temp;
    data_temp = data;
    for (qint32 j = 0; j < 3; j++) {
        for (qint32 i = 1; i < N - 1; i++) {
            data_temp[i][j]
                = (a1 * data[i - 1][j] +
                   a2 * data[i][j] +
                   a3 * data[i + 1][j]) / total;
        }
    }
    data = data_temp;
}

void FormEdgeAdjustment::SetCursorChange() {
    double scale = select_render_->GetActiveCamera()->GetParallelScale() / 500;
    qint32 tmp = static_cast<int>(line_adjustment_size_ / scale * 2);
    if (ui->widget->height() - 50 < tmp) {
        tmp =  ui->widget->height() - 50;
    }
    QPixmap tmp_png = QPixmap(
                          QString("djustment.png"));
    ui->widget->setCursor(QCursor(tmp_png.scaled(tmp, tmp,
                                  Qt::KeepAspectRatio, Qt::SmoothTransformation)
                                  , - 1, -1));
}

void FormEdgeAdjustment::splinefiltering(
    QList<QList<double> > &data, qint32 begin, qint32 end) {
    qint32 a1, a2, a3, total, N;
    a1 = 1;
    a2 = 1;
    a3 = 1;
    total = a1 + a2 + a3;
    N = data.size();
    if (begin - 5 > 1) {
        begin -= 5;
    } else {
        begin = 1;
    }
    if (end + 5 < N - 1) {
        end += 5;
    } else {
        end = N - 1;
    }
    QList<QList<double>> data_temp;
    data_temp = data;
    for (qint32 j = 0; j < 3; j++) {
        for (qint32 i = begin; i < end; i++) {
            data_temp[i][j]
                = (a1 * data[i - 1][j] +
                   a2 * data[i][j] +
                   a3 * data[i + 1][j]) / total;
        }
    }
    data = data_temp;
}

#include "vtk_pediclescrew_source.h"

#include <vtkAppendPolyData.h>
#include <vtkCellArray.h>
#include <vtkCylinderSource.h>
#include <vtkFloatArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangle.h>

vtkStandardNewMacro(VtkPedicleScrewSource);

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

VtkPedicleScrewSource::VtkPedicleScrewSource(int)
{
    this->OutputPointsPrecision = SINGLE_PRECISION;
    this->SetNumberOfInputPorts(0);
    this->PedicleFileName = nullptr;
}

VtkPedicleScrewSource::~VtkPedicleScrewSource()
{
    this->SetPedicleFileName(nullptr);
}

int VtkPedicleScrewSource::RequestData(
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector ** vtkNotUsed(inputVector),
  vtkInformationVector * outputVector)
{
    vtkInformation * outInfo = outputVector->GetInformationObject(0);
    vtkPolyData * output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    //
    std::vector<int> triangle_cell {
        0, 2, 6, 0, 4, 6, 1, 3, 7, 1, 5, 7,
        0, 1, 5, 0, 4, 5, 2, 3, 7, 2, 6, 7,
        0, 1, 3, 0, 2, 3, 0, 1, 5, 0, 4, 5,
        0, 2, 6, 0, 4, 6, 2, 3, 7, 2, 6, 7,
        1, 5, 0, 3, 7, 0
    };

    // 螺钉---螺纹
    vtkNew<vtkPolyData> screw_1;
    vtkNew<vtkPoints> points_1;
    vtkNew<vtkCellArray> cells_1;
    vtkNew<vtkTriangle> trianle;
    double Omega = M_PI * 2.0 / Subdivision; // 角速度
    double H = (Length - 16) * TopR / (TopR - BottonmR); // 圆锥高度
    vtkIdType number = static_cast<vtkIdType>(H * (TopR - BottonmR) / (V * TopR)); // 螺旋线点总数
    int begin_t = static_cast<int>(H * BottonmR / (V * TopR)); // 螺旋线起始时间（圆台底部）
    // 螺钉---螺纹---四条螺旋线 points
    for (int i = 0; i < number; i++) {
        int T = begin_t + i;
        double x = TopR * V * T / H * cos(Omega * T);
        double y = TopR * V * T / H * sin(Omega * T);
        double z = V * T + Ascending;
        points_1->InsertNextPoint(x, y, z);
        z -= D + Ascending * 2;
        points_1->InsertNextPoint(x, y, z);
        if (i < Subdivision * 2) {
            x += TopR * cos(Omega * T) * (1 - V * T / H) * (i + 1) / (Subdivision * 2);
            y += TopR * sin(Omega * T) * (1 - V * T / H) * (i + 1) / (Subdivision * 2);
        } else {
            x = TopR * cos(Omega * T);
            y = TopR * sin(Omega * T);
        }
        z = V * T;
        points_1->InsertNextPoint(x, y, z);
        z -= D;
        points_1->InsertNextPoint(x, y, z);
    }
    // 螺钉---螺纹---螺纹 cells
    for (int i = 0; i < number * 4 - 4; i = i + 4) {
        for (unsigned long long j = 0; j < 8; j++) {
            trianle->GetPointIds()->SetId(0, i + triangle_cell.at(j * 3 + 0));
            trianle->GetPointIds()->SetId(1, i + triangle_cell.at(j * 3 + 1));
            trianle->GetPointIds()->SetId(2, i + triangle_cell.at(j * 3 + 2));
            cells_1->InsertNextCell(trianle);
        }
    }
    screw_1->SetPoints(points_1);
    screw_1->SetPolys(cells_1);
    // 螺钉---圆台
    vtkNew<vtkPolyData> screw_2;
    vtkNew<vtkPoints> points_2;
    vtkNew<vtkCellArray> cells_2;
    for (int i = 0; i < Subdivision; i++) {
        double x = BottonmR * cos(Omega * i);
        double y = BottonmR * sin(Omega * i);
        double z = V * begin_t - Ascending - D;
        points_2->InsertNextPoint(x, y, z);
        x = (BottonmR - Chamfering) * cos(Omega * i);
        y = (BottonmR - Chamfering) * sin(Omega * i);
        z -= Chamfering;
        points_2->InsertNextPoint(x, y, z);
        x = TopR * cos(Omega * i);
        y = TopR * sin(Omega * i);
        z = V * (static_cast<double>(begin_t + number)) + Ascending;
        points_2->InsertNextPoint(x, y, z);
        x = (TopR - Chamfering) * cos(Omega * i);
        y = (TopR - Chamfering) * sin(Omega * i);
        z += Chamfering;
        points_2->InsertNextPoint(x, y, z);
    }
    double x = BottonmR * cos(Omega * 0);
    double y = BottonmR * sin(Omega * 0);
    double z = V * begin_t - Ascending - D;
    points_2->InsertNextPoint(x, y, z);
    x = (BottonmR - Chamfering) * cos(Omega * 0);
    y = (BottonmR - Chamfering) * sin(Omega * 0);
    z -= Chamfering;
    points_2->InsertNextPoint(x, y, z);
    x = TopR * cos(Omega * 0);
    y = TopR * sin(Omega * 0);
    z = V * (static_cast<double>(begin_t + number)) + Ascending;
    points_2->InsertNextPoint(x, y, z);
    x = (TopR - Chamfering) * cos(Omega * 0);
    y = (TopR - Chamfering) * sin(Omega * 0);
    z += Chamfering;
    points_2->InsertNextPoint(x, y, z);
    points_2->InsertNextPoint(0, 0, V * begin_t - Ascending - Chamfering - D);
    points_2->InsertNextPoint(0, 0, V * (static_cast<double>(begin_t + number)) + Ascending + Chamfering);
    for (unsigned long long j = 8; j < 10; j++) {
        trianle->GetPointIds()->SetId(0, triangle_cell.at(j * 3 + 0));
        trianle->GetPointIds()->SetId(1, triangle_cell.at(j * 3 + 1));
        trianle->GetPointIds()->SetId(2, triangle_cell.at(j * 3 + 2));
        cells_2->InsertNextCell(trianle);
    }
    for (int i = 0; i < Subdivision; i++) {
        vtkIdType offset = i * 4;
        for (unsigned long long j = 10; j < 16; j++) {
            trianle->GetPointIds()->SetId(0, offset + triangle_cell.at(j * 3 + 0));
            trianle->GetPointIds()->SetId(1, offset + triangle_cell.at(j * 3 + 1));
            trianle->GetPointIds()->SetId(2, offset + triangle_cell.at(j * 3 + 2));
            cells_2->InsertNextCell(trianle);
        }
        trianle->GetPointIds()->SetId(0, offset + triangle_cell.at(16 * 3 + 0));
        trianle->GetPointIds()->SetId(1, offset + triangle_cell.at(16 * 3 + 1));
        trianle->GetPointIds()->SetId(2, (Subdivision + 1) * 4);
        cells_2->InsertNextCell(trianle);
        trianle->GetPointIds()->SetId(0, offset + triangle_cell.at(17 * 3 + 0));
        trianle->GetPointIds()->SetId(1, offset + triangle_cell.at(17 * 3 + 1));
        trianle->GetPointIds()->SetId(2, (Subdivision + 1) * 4 + 1);
        cells_2->InsertNextCell(trianle);
    }
    screw_2->SetPoints(points_2);
    screw_2->SetPolys(cells_2);
    // 螺钉
    vtkNew<vtkAppendPolyData> screw_append;
    screw_append->AddInputData(screw_2);
    screw_append->AddInputData(screw_1);
    vtkNew<vtkTransform> screw_translation;
    screw_translation->Translate(0, 0, -1 * (V * (static_cast<double>(begin_t + number)) + Ascending + Chamfering));
    vtkNew<vtkTransformPolyDataFilter> screw_transform;
    screw_transform->SetInputConnection(screw_append->GetOutputPort());
    screw_transform->SetTransform(screw_translation);
    // 椎弓根
    vtkNew<vtkPolyData> pedicle;
    vtkNew<vtkSTLReader> pedicle_reader;
    pedicle_reader->SetFileName(PedicleFileName);
    vtkNew<vtkTransform> pedicle_translation;
    pedicle_translation->Translate(PedicleTranslateX, PedicleTranslateY, PedicleTranslateZ);
    pedicle_translation->RotateX(PedicleRotateX);
    vtkNew<vtkTransformPolyDataFilter> pedicle_transform;
    pedicle_transform->SetInputConnection(pedicle_reader->GetOutputPort());
    pedicle_transform->SetTransform(pedicle_translation);
    // 引导线
    vtkNew<vtkCylinderSource> guide_line;
    guide_line->SetHeight(GuideHeight);
    guide_line->SetCenter(0, -0.5 * GuideHeight, 0);
    guide_line->SetRadius(0.2);
    vtkNew<vtkTransform> guide_translation;
    guide_translation->RotateX(-90);
    vtkNew<vtkTransformPolyDataFilter> guide_transform;
    guide_transform->SetInputConnection(guide_line->GetOutputPort());
    guide_transform->SetTransform(guide_translation);
    // 组合
    vtkNew<vtkAppendPolyData> out_append;
    out_append->AddInputConnection(screw_transform->GetOutputPort());
    out_append->AddInputConnection(pedicle_transform->GetOutputPort());
    out_append->AddInputConnection(guide_transform->GetOutputPort());
    // 螺栓是按照 xy->z 画的，显示是xz->y
    out_append->Update();
    double center[3];
    out_append->GetOutput()->GetCenter(center);
    vtkNew<vtkTransform> out_translation;
    out_translation->PostMultiply();
    out_translation->Translate(0, 0, center[2]);
    out_translation->RotateX(90);
    out_translation->Translate(0, center[2], 0);
    out_translation->Translate(0, -Length + 16 - 2 * D - Chamfering - Ascending, 0);
    vtkNew<vtkTransformPolyDataFilter> out_transform;
    out_transform->SetInputConnection(out_append->GetOutputPort());
    out_transform->SetTransform(out_translation);
    out_transform->Update();
    output->DeepCopy(out_transform->GetOutput());
    return 1;
}

void VtkPedicleScrewSource::PrintSelf(ostream & os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Subdivision: " << this->Subdivision << "\n";
    os << indent << "TopR: " << this->TopR << "\n";
    os << indent << "Length: " << this->Length << "\n";
    os << indent << "V: " << this->V << "\n";
    os << indent << "D: " << this->D << "\n";
    os << indent << "BottonmR: " << this->BottonmR << "\n";
    os << indent << "Chamfering: " << this->Chamfering << "\n";
    os << indent << "Ascending: " << this->Ascending << "\n";
    os << indent << "GuideHeight: " << this->GuideHeight << "\n";
    os << indent << "PedicleRotateX: " << this->PedicleRotateX << "\n";
    os << indent << "PedicleTranslateX: " << this->PedicleTranslateX << "\n";
    os << indent << "PedicleTranslateY: " << this->PedicleTranslateY << "\n";
    os << indent << "PedicleTranslateZ: " << this->PedicleTranslateZ << "\n";
    os << indent << "PedicleFileName: " << this->PedicleFileName << "\n";
}

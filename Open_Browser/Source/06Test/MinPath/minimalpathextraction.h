#ifndef MINIMALPATHEXTRACTION_H
#define MINIMALPATHEXTRACTION_H
#include "stable.h"
#include <iostream>
#include "itkNumericTraits.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkPolyLineParametricPath.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkArrivalFunctionToPathFilter.h"
#include "itkSpeedFunctionToPathFilter.h"
#include "itkPathIterator.h"
#include "itkGradientDescentOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkIterateNeighborhoodOptimizer.h"
#include <itkRescaleIntensityImageFilter.h>
#include "itkCastImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImageKernelOperator.h"


class MinimalPathExtraction {
  public:
    static bool gradientdescent(
        const char &png_name_constchar,
        QList<QPointF> path_points,
        QList<QPointF> &point_list) {
        // 类型定义
        constexpr unsigned int Dimension = 2;
        using FloatType = float  ;
        using CharType = unsigned char  ;
        using ImageFloatType = itk::Image< FloatType, Dimension >;
        using ImageCharType = itk::Image< CharType, Dimension >;
        using ReaderType = itk::ImageFileReader< ImageFloatType >;
        using PathType = itk::PolyLineParametricPath< Dimension >;
        using PathFilterType =
            itk::SpeedFunctionToPathFilter< ImageFloatType, PathType >;
        using CoordRepType = PathFilterType::CostFunctionType::CoordRepType;
        //读取速度功能
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(&png_name_constchar);
        reader->Update();
        ImageFloatType::Pointer speed = reader->GetOutput();
        speed->DisconnectPipeline();
        ImageFloatType::SizeType size = speed->GetLargestPossibleRegion().GetSize();
        ImageFloatType::IndexType pixelIndex;
        for (int i = 0; i < static_cast<int>(size[0]); i++) {
            for (int j = 0; j < static_cast<int>(size[1]); j++) {
                pixelIndex[0] = i;
                pixelIndex[1] = j;
                speed->SetPixel(pixelIndex, 1 - (speed->GetPixel(pixelIndex) / 255));

            }
        }
        // 创建插值器
        using InterpolatorType =
            itk::LinearInterpolateImageFunction<ImageFloatType, CoordRepType>;
        InterpolatorType::Pointer interp = InterpolatorType::New();
        // 创建成本函数
        PathFilterType::CostFunctionType::Pointer cost =
            PathFilterType::CostFunctionType::New();
        cost->SetInterpolator(interp);
        // 创建优化器
        using OptimizerType = itk::GradientDescentOptimizer;
        OptimizerType::Pointer optimizer = OptimizerType::New();
        optimizer->SetNumberOfIterations(1000);
        // 创建路径筛选器
        PathFilterType::Pointer pathFilter = PathFilterType::New();
        pathFilter->SetInput(speed);
        pathFilter->SetCostFunction(cost);
        pathFilter->SetOptimizer(optimizer);
        pathFilter->SetTerminationValue(1.8);
        // 设置路径点 && 添加路径信息
        PathFilterType::PointType start, end, way_tmp;
        //QList<PathFilterType::PointType> way;
        using PathInformationType = PathFilterType::PathInformationType;
        PathInformationType::Pointer info = PathInformationType::New();
        start[0] = path_points.at(0).x();
        start[1] = path_points.at(0).y();
        info->SetStartPoint(start);
        for (int i = 1; i < path_points.count() - 1; i++) {
            way_tmp[0] = path_points.at(i).x();
            way_tmp[1] = path_points.at(i).y();
            info->AddWayPoint(way_tmp);
        }
        end[0] = path_points.at(path_points.count() - 1).x();
        end[1] = path_points.at(path_points.count() - 1).y();
        info->SetEndPoint(end);
        pathFilter->AddPathInformation(info);
        // 计算路径
        pathFilter->Update();
        // 光栅化路径
        for (unsigned int i = 0; i < pathFilter->GetNumberOfOutputs(); i++) {
            PathType::Pointer path = pathFilter->GetOutput(i);
            // 检查路径有效
            if (path->GetVertexList()->Size() == 0) {
                std::cout << "WARNING: Path " << (i + 1) <<
                          " contains no points!" << std::endl;
                continue;
            }
            qreal x, y;
            QMap <int, QMap< int, QPointF>> tmp_map;
            QMap<int, QPointF> tmp_point;
            const PathType::VertexListType *vertexList = path->GetVertexList();
            for (unsigned int i = 0; i < vertexList->Size(); i = i + 2) {
                x = vertexList->GetElement(i)[0];
                y = vertexList->GetElement(i)[1];
                point_list << QPointF(x, y);

            }
        }
        return EXIT_SUCCESS;
    }
};
#endif // MINIMALPATHEXTRACTION_H

























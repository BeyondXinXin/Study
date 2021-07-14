#ifndef VTKPEDICLESCREWSOURCE_H
#define VTKPEDICLESCREWSOURCE_H

#include <vtkCell.h>
#include <vtkFiltersSourcesModule.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkSetGet.h>

/**
 * @brief The VtkPedicleScrewSource class
 * 生成 椎弓根螺钉 数据
 */
class VtkPedicleScrewSource : public vtkPolyDataAlgorithm
{
public:
    static VtkPedicleScrewSource * New();
    vtkTypeMacro(VtkPedicleScrewSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream & os, vtkIndent indent) override;

    vtkSetMacro(Subdivision, int);
    vtkGetMacro(Subdivision, int);
    vtkSetMacro(TopR, double);
    vtkGetMacro(TopR, double);
    vtkSetMacro(Length, double);
    vtkGetMacro(Length, double);
    vtkSetMacro(V, double);
    vtkGetMacro(V, double);
    vtkSetMacro(D, double);
    vtkGetMacro(D, double);
    vtkSetMacro(BottonmR, double);
    vtkGetMacro(BottonmR, double);
    vtkSetMacro(Chamfering, double);
    vtkGetMacro(Chamfering, double);
    vtkSetMacro(Ascending, double);
    vtkGetMacro(Ascending, double);
    vtkSetMacro(GuideHeight, double);
    vtkGetMacro(GuideHeight, double);
    vtkSetMacro(PedicleRotateX, double);
    vtkGetMacro(PedicleRotateX, double);
    vtkSetMacro(PedicleTranslateX, double);
    vtkGetMacro(PedicleTranslateX, double);
    vtkSetMacro(PedicleTranslateY, double);
    vtkGetMacro(PedicleTranslateY, double);
    vtkSetMacro(PedicleTranslateZ, double);
    vtkGetMacro(PedicleTranslateZ, double);
    vtkSetStringMacro(PedicleFileName);
    vtkGetStringMacro(PedicleFileName);

protected:
    VtkPedicleScrewSource(int res = 6);
    ~VtkPedicleScrewSource() override;
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;

protected: // 遵循vtk命名
    int OutputPointsPrecision;
    int Subdivision = 40; // 每一圈螺纹细分数量
    double TopR = 3.0; // 圆台顶部半径
    double Length = 55.0; // 总长度
    double V = 0.06; // 螺旋线前进速度
    double D = 0.5; // 螺纹宽度
    double BottonmR = 1.0; // 圆台底部半径
    double Chamfering = 0.3; // 螺纹倒角
    double Ascending = 0.3; // 螺栓倒角
    double GuideHeight = 150; // 引导线高度

    // 模型坐标系乱选的，让他们最后重新提供吧
    double PedicleRotateX = 180;
    double PedicleTranslateX = -5;
    double PedicleTranslateY = 7;
    double PedicleTranslateZ = 18;
    char * PedicleFileName; // 椎弓根名称

private:
    VtkPedicleScrewSource(const VtkPedicleScrewSource &) = delete;
    void operator=(const VtkPedicleScrewSource &) = delete;
};

#endif // VTKPEDICLESCREWSOURCE_H

#include <QApplication>
#include <QTabWidget>

#include <class1/chapter1/Chapter1>
#include <class1/chapter2/Chapter2>
#include <class1/chapter3/Chapter3>
#include <class1/chapter4/Chapter4>

QTabWidget *GetClass1();
QTabWidget *GetClass2();
QTabWidget *GetClass3();
QTabWidget *GetClass4();
QTabWidget *GetClass5();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTabWidget main_tab;

    main_tab.addTab(GetClass1(), u8"1 Qt 实现版");
    main_tab.addTab(GetClass2(), u8"2 光照");
    main_tab.addTab(GetClass3(), u8"3 模型加载");
    main_tab.addTab(GetClass4(), u8"4 高级OpenGL");
    main_tab.addTab(GetClass5(), u8"4 高级光照");

    int i = 0, j = 3, k = 0;
    main_tab.setCurrentIndex(i);
    auto cur_chapter_wid = qobject_cast<QTabWidget *>(main_tab.currentWidget());
    cur_chapter_wid->setCurrentIndex(j);
    auto cur_wid = qobject_cast<QTabWidget *>(cur_chapter_wid->currentWidget());
    cur_wid->setCurrentIndex(k);

    main_tab.resize(1800, 1000);
    main_tab.setWindowTitle(u8"Learn OpenGL");
    main_tab.show();

    return a.exec();
}

QTabWidget *GetClass1()
{
    QTabWidget *tab_class = new QTabWidget();
    // -------- 创建窗口 --------
    QTabWidget *chap1 = new QTabWidget();
    tab_class->addTab(chap1, u8"1.1 创建窗口");
    GLWidget_1_1 *wid_1_1 = new GLWidget_1_1();
    chap1->addTab(wid_1_1, u8"1 创建窗口");
    GLWidget_1_2 *wid_1_2 = new GLWidget_1_2();
    chap1->addTab(wid_1_2, u8"2 创建三角形");
    GLWidget_1_3 *wid_1_3 = new GLWidget_1_3();
    chap1->addTab(wid_1_3, u8"3 EBO");
    GLWidget_1_4 *wid_1_4 = new GLWidget_1_4();
    chap1->addTab(wid_1_4, u8"4 练习");

    QTabWidget *chap2 = new QTabWidget();
    tab_class->addTab(chap2, u8"1.2 Qt交互与Shader");
    GLWidget_2_1 *wid_2_1 = new GLWidget_2_1();
    chap2->addTab(wid_2_1, u8"1 调用OpenGL控件");
    GLWidget_2_2 *wid_2_2 = new GLWidget_2_2();
    chap2->addTab(wid_2_2, u8"2 QOpenGLShaderProgram");
    GLWidget_2_3 *wid_2_3 = new GLWidget_2_3();
    chap2->addTab(wid_2_3, u8"3 GLSL_in_out");
    GLWidget_2_4 *wid_2_4 = new GLWidget_2_4();
    chap2->addTab(wid_2_4, u8"4 GLSL_layout");
    GLWidget_2_5 *wid_2_5 = new GLWidget_2_5();
    chap2->addTab(wid_2_5, u8"5 GLSL_uniform");
    GLWidget_2_6 *wid_2_6 = new GLWidget_2_6();
    chap2->addTab(wid_2_6, u8"6 test");

    QTabWidget *chap3 = new QTabWidget();
    tab_class->addTab(chap3, u8"1.3 纹理");
    GLWidget_3_1 *wid_3_1 = new GLWidget_3_1();
    chap3->addTab(wid_3_1, u8"1 加载纹理、纹理单元");
    GLWidget_3_2 *wid_3_2 = new GLWidget_3_2();
    chap3->addTab(wid_3_2, u8"2 环绕方式、纹理过滤、多级渐远纹理");
    GLWidget_3_3 *wid_3_3 = new GLWidget_3_3();
    chap3->addTab(wid_3_3, u8"3 练习");

    QTabWidget *chap4 = new QTabWidget();
    tab_class->addTab(chap4, u8"1.4 变换");
    GLWidget_4_1 *wid_4_1 = new GLWidget_4_1();
    chap4->addTab(wid_4_1, u8"1 旋转、平移");

    QTabWidget *chap5 = new QTabWidget();
    tab_class->addTab(chap5, u8"1.5 坐标系统");

    QTabWidget *chap6 = new QTabWidget();
    tab_class->addTab(chap6, u8"1.6 摄像机");

    return tab_class;
}

QTabWidget *GetClass2()
{
    QTabWidget *tab_class = new QTabWidget();

    return tab_class;
}

QTabWidget *GetClass3()
{
    QTabWidget *tab_class = new QTabWidget();
    return tab_class;
}

QTabWidget *GetClass4()
{
    QTabWidget *tab_class = new QTabWidget();
    return tab_class;
}

QTabWidget *GetClass5()
{
    QTabWidget *tab_class = new QTabWidget();
    return tab_class;
}

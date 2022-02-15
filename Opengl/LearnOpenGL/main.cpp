#include <QApplication>
#include <QTabWidget>

#include <chapter1/Chapter1>

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

    main_tab.resize(800, 600);
    main_tab.setWindowTitle(u8"Learn OpenGL");
    main_tab.show();

    return a.exec();
}

QTabWidget *GetClass1()
{
    QTabWidget *tab_class_1 = new QTabWidget();
    // -------- 创建窗口 --------
    QTabWidget *chap1 = new QTabWidget();
    tab_class_1->addTab(chap1, u8"1.1 创建窗口");
    GLWidget_1_1 *wid_1_1 = new GLWidget_1_1();
    chap1->addTab(wid_1_1, u8"1 创建窗口");
    GLWidget_1_2 *wid_1_2 = new GLWidget_1_2();
    chap1->addTab(wid_1_2, u8"2 创建三角形");
    GLWidget_1_3 *wid_1_3 = new GLWidget_1_3();
    chap1->addTab(wid_1_3, u8"3 EBO");
    GLWidget_1_4 *wid_1_4 = new GLWidget_1_4();
    chap1->addTab(wid_1_4, u8"4 练习");

    QTabWidget *chap2 = new QTabWidget();
    tab_class_1->addTab(chap2, u8"1.2 Qt交互与Shader");

    QTabWidget *chap3 = new QTabWidget();
    tab_class_1->addTab(chap3, u8"1.3 纹理");

    QTabWidget *chap4 = new QTabWidget();
    tab_class_1->addTab(chap4, u8"1.4 变换");

    QTabWidget *chap5 = new QTabWidget();
    tab_class_1->addTab(chap5, u8"1.5 坐标系统");

    QTabWidget *chap6 = new QTabWidget();
    tab_class_1->addTab(chap6, u8"1.6 摄像机");

    return tab_class_1;
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

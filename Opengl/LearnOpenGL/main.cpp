#include <QApplication>
#include <QTabWidget>

#include <chapter1/Chapter1>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTabWidget main_tab;

    QTabWidget chap1;
    main_tab.addTab(&chap1, u8"创建窗口");
    QTabWidget chap2;
    main_tab.addTab(&chap2, u8"Qt交互与Shader");
    QTabWidget chap3;
    main_tab.addTab(&chap3, u8"纹理");
    QTabWidget chap4;
    main_tab.addTab(&chap4, u8"变换");
    QTabWidget chap5;
    main_tab.addTab(&chap5, u8"坐标系统");
    QTabWidget chap6;
    main_tab.addTab(&chap5, u8"摄像机");

    GLWidget_1_1 wid_1_1;
    chap1.addTab(&wid_1_1, u8"创建窗口");

    GLWidget_1_2 wid_1_2;
    chap1.addTab(&wid_1_2, u8"创建三角形");

    GLWidget_1_3 wid_1_3;
    chap1.addTab(&wid_1_3, u8"EBO");

    GLWidget_1_1 wid_1_4;
    chap1.addTab(&wid_1_4, u8"练习");

    main_tab.resize(800, 600);
    main_tab.setWindowTitle(u8"Opengl入门");
    main_tab.show();

    return a.exec();
}

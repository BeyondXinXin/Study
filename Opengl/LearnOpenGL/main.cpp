#include <QApplication>
#include <QTabWidget>

#include <chapter1/Chapter1>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTabWidget main_tab;

    QTabWidget chap1;
    main_tab.addTab(&chap1, u8"��������");
    QTabWidget chap2;
    main_tab.addTab(&chap2, u8"Qt������Shader");
    QTabWidget chap3;
    main_tab.addTab(&chap3, u8"����");
    QTabWidget chap4;
    main_tab.addTab(&chap4, u8"�任");
    QTabWidget chap5;
    main_tab.addTab(&chap5, u8"����ϵͳ");
    QTabWidget chap6;
    main_tab.addTab(&chap5, u8"�����");

    GLWidget_1_1 wid_1_1;
    chap1.addTab(&wid_1_1, u8"��������");

    GLWidget_1_2 wid_1_2;
    chap1.addTab(&wid_1_2, u8"����������");

    GLWidget_1_3 wid_1_3;
    chap1.addTab(&wid_1_3, u8"EBO");

    GLWidget_1_1 wid_1_4;
    chap1.addTab(&wid_1_4, u8"��ϰ");

    main_tab.resize(800, 600);
    main_tab.setWindowTitle(u8"Opengl����");
    main_tab.show();

    return a.exec();
}

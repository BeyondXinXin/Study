#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <custom_item.h>
#include "stable.h"

namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void OpenMinPathWidgetIn(const QImage background);
  private slots:
    void SlotSeekMinPathWidgetIn();
  private:
    void Initial();
  private:
    Ui::MainWindow *ui;
    MyCustomQGraphicsScene *setup_scene_;
    MyCustomIQGraphicsPixmapItem *scene_background_;
    QImage img_background_;
    MyCustomIQGraphicsPathItem *center_path_item_;
    bool can_seek_;
};

#endif // MAINWINDOW_H

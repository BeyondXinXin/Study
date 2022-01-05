#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <minimalpathextraction.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    can_seek_(false) {
    ui->setupUi(this);
    this->setWindowTitle("最短路径提取测试");
    this->Initial();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SlotSeekMinPathWidgetIn() {
    QPointF tmp_f =
        QPointF(img_background_.width() / 2, img_background_.height() / 2);
    QFont myFont;
    QList<QPointF> tmp_list_pointf;
    if (setup_scene_->SLotGetItemSpoint(
                tmp_list_pointf, tmp_list_pointf, scene_background_->pos())) {
        std::string tmp_bya = QString("minpath.png").toStdString();
        const char *png_name_constchar = tmp_bya.c_str();
        QList<QPointF>point_list;
        if (MinimalPathExtraction::gradientdescent(
                    *png_name_constchar, tmp_list_pointf, point_list)) {
            return;
        }
        QPainterPath starPath;
        starPath.moveTo(QPointF(point_list.at(0).x(),
                                point_list.at(0).y()) + tmp_f);
        for (int i = 0; i < point_list.count(); i ++) {
            starPath.lineTo(QPointF(point_list.at(i).x(),
                                    point_list.at(i).y()) + tmp_f);
        }
        center_path_item_->setPath(starPath);
        center_path_item_->show();
    }
}


void MainWindow::OpenMinPathWidgetIn(const QImage background) {
    img_background_ = background;
    can_seek_ = true;
    scene_background_->setPixmap(QPixmap::fromImage(img_background_));
    setup_scene_->setSceneRect(0, 0,
                               img_background_.width() * 2,
                               img_background_.height() * 2);
    setup_scene_->SlotDalteAllItem();
    scene_background_->moveBy(img_background_.width() /
                              2 - scene_background_->pos().x(),
                              img_background_.height() /
                              2 - scene_background_-> pos().y());
    ui->graphicsView->setScene(setup_scene_);
    ui->graphicsView->show();
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->scale(1.1, 1.1);
    ui->stackedWidget->setCurrentIndex(0);
    center_path_item_->hide();
    QImage baseImage = background;
    baseImage.save("minpath.png");
    this->showMaximized();
}

void MainWindow::Initial() {
    setup_scene_ = new MyCustomQGraphicsScene;
    scene_background_ = new MyCustomIQGraphicsPixmapItem();
    center_path_item_ = new MyCustomIQGraphicsPathItem;
    setup_scene_->addItem(center_path_item_);
    setup_scene_->addItem(scene_background_);
    connect(setup_scene_, &MyCustomQGraphicsScene::SignalBtnSeekOut,
            this, &MainWindow::SlotSeekMinPathWidgetIn);
    this->hide();
}





















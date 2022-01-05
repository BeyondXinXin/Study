#include "dialog.h"
#include "ui_dialog.h"
#include "quickapplication.h"

#include "student.h"
#include <QDebug>
#include <QEventLoop>
#include <QByteArray>
#include <QDateTime>
#include <QTimer>
#include <QJsonObject>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
    QuickApplication::subscibeEvent(this, "show_time");
    QuickApplication::subscibeEvent(this, "dashao");
    QUICK_SUBSCIBE_H("denglu", 10)
    qDebug() << "--------100-------";
    box = new QMessageBox();
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::event_show_time(const QDateTime &time) {
    box->setText(time.toString());
    box->show();
}

void Dialog::on_pushButton_clicked() {
    Student student;
    student.name_ = ui->name_lineEdit->text();
    student.id_ = ui->id_lineEdit->text();
    ui->show_label->setText("登录...");
    QuickApplication::publishEvent("denglu", Qt::BlockingQueuedConnection, student);
}

void Dialog::on_pushButton_2_clicked() {
    QuickApplication::exit();
}

void Dialog::on_pushButton_3_clicked() {
    QuickApplication::publishEvent("dashao", Qt::AutoConnection, ui->name_lineEdit->text(), QString("打扫卫生"));
}

void Dialog::on_pushButton_4_clicked() {
    QuickController::destory();
    qApp->exit(0);
}

void Dialog::event_dashao(const QString &name, QString work) {
    QuickApplication::publishEvent("linggongju", Qt::AutoConnection, name);
    qDebug() <<  name << ":" << work;
}

void Dialog::event_denglu(const Student &stu) {
    qDebug() << "***********-***************";
}

#ifndef DIALOG_H
#define DIALOG_H

#include "quick.h"
#include "quickevent.h"
#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include "student.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
    QUICK_EVENT(QDialog)
  public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog() override;
  public slots:
    void event_show_time(const QDateTime &time);
    void on_pushButton_clicked();
  private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    //
    void event_dashao(const QString &name, QString work);
    void event_denglu(const Student &stu);
  private:
    Ui::Dialog *ui;
    QMessageBox *box;
};

#endif // DIALOG_H

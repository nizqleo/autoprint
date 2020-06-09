#ifndef PRINTING_H
#define PRINTING_H

#include <QWidget>
#include "pages\dialog.h"
#include "models\ordermodel.h"
#include <QTimer>
#include <QTime>

#include <QMessageBox>
namespace Ui {
class Printing;
}

class Printing : public QWidget
{
    Q_OBJECT

public:
    explicit Printing(OrderModel* OM, QWidget* SW, QWidget *parent = nullptr);
    ~Printing();

    void send_AR4_to_printer();


signals:
    void go_on_to_next_job();
    void finish_sending();
    void start_printing();
    void finish_printing();

private slots:
    void on_Stop_button_clicked();
    void slotfor_state_update();
    void slotfor_finish_uploading();
    void slotfor_starting_printing();
    void slotfor_finishing_printing();



private:
    Ui::Printing *ui;
    QWidget* SW;
    OrderModel* OM;
    int finishedJobs;
    int TotalJobs;
    int currentOrderIndex;
    QString lastPattern;
    QString pattern;
    QString cloth;
    QTime time;


};

#endif // PRINTING_H

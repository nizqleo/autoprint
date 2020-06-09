#ifndef PRINTERSETTING_H
#define PRINTERSETTING_H

#include <QWidget>
#include <QMainWindow>

class MainWindow;
namespace Ui {
class printerSetting;
}

class printerSetting : public QWidget
{
    Q_OBJECT

public:
    explicit printerSetting(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~printerSetting();

private slots:
    void on_taskPage_button_clicked();
    void on_dataPage_button_clicked();

signals:
    void taskpageopen();
    void datapageopen();


private:
    Ui::printerSetting *ui;
    MainWindow *MW;


};

#endif // PRINTERSETTING_H

#ifndef PRINTERSETTING_H
#define PRINTERSETTING_H

#include <QWidget>
#include <QMainWindow>

#include "models\ordermodel.h"
#include <QTableView>
#include "models\datasetmodel.h"
#include <QDebug>
#include "pages\printtaskmanagement.h"
#include "models\task.h"



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
    void on_Add_button_clicked();
    void on_Delete_button_clicked();
    void on_Edit_button_clicked();
    void on_Exit_button_clicked();

    void save_files(QString pattern, QString DFAR4Address, QString DBAR4Address,
                    QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                    QString MimageAddress, bool hasBack, bool hasFront);
    void delete_files(QString pattern);
    void on_tableView_clicked();

    void on_taskPage_button_clicked();
    void on_dataPage_button_clicked();
    void on_search_lineEdit_textEdited(QString s);

signals:
    void taskpageopen();
    void datapageopen();


private:
    Ui::printerSetting *ui;
    MainWindow *MW;



};

#endif // PRINTERSETTING_H

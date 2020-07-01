#ifndef DATAMAINTAINANCE_H
#define DATAMAINTAINANCE_H

#include <QWidget>
#include <QMainWindow>
#include <QStandardItem>
#include "models\ordermodel.h"
#include <QTableView>
#include "models\datasetmodel.h"
#include "dialog.h"
#include <QDebug>
#include "pages\printtaskmanagement.h"
#include "pages\printersetting.h"
#include "models\task.h"

class MainWindow;
namespace Ui {
class dataMaintainance;
}

class dataMaintainance : public QWidget
{
    Q_OBJECT

public:
    explicit dataMaintainance(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~dataMaintainance();

    QString searchDir;
    datasetModel* DModel;
    bool patternNameOverlapCheck(QString s);

private slots:
    void on_Add_button_clicked();
    void on_Delete_button_clicked();
    void on_Edit_button_clicked();
    void on_Exit_button_clicked();

    void delete_files(QString pattern);

    void on_tableView_clicked();

    void on_taskPage_button_clicked();
    void on_settingPage_button_clicked();

    void on_search_lineEdit_textEdited(QString s);

    /* add new pattern to Model
     * and add totalnum
     */
    void AddNewPattern(Pattern* pattern);
    void AddNewPattern(QString pattern, int type);
signals:
    void taskpageopen();
    void settingpageopen();


private:
    Ui::dataMaintainance *ui;
    MainWindow *MW;

    Dialog* dialog;
};


#endif // DATAMAINTAINANCE_H

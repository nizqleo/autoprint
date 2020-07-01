#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>

class printTaskManagement;
namespace Ui {
class DashBoard;
}

class DashBoard : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr);
   DashBoard(printTaskManagement* _PTM, QWidget *parent = nullptr);
    ~DashBoard();

public slots:
   void updateTableViews();

private:
    Ui::DashBoard *ui;

    printTaskManagement* PTM;

};

#endif // DASHBOARD_H

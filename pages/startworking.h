#ifndef STARTWORKING_H
#define STARTWORKING_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include <QWidget>
#include <QMainWindow>
#include "models\ordermodel.h"
#include "pickuplist.h"
#include "printing.h"
#include "auxiliary.h"


using namespace std;

namespace Ui {
class startWorking;
}

class startWorking : public QWidget
{
    Q_OBJECT

public:
    explicit startWorking(QMainWindow* MW, QWidget *parent = nullptr);
    ~startWorking();
    QMainWindow *MW;
    int SizeMap(string size);
    int ColorMap(string color);

private slots:
    void on_PL_button_clicked();
    void on_CP_button_clicked();
    void on_Return_button_clicked();
    void on_lastPattern_button_clicked();
    void on_nextPattern_button_clicked();


private:
    Ui::startWorking *ui;
    OrderModel* OM;
    QMap<QString, int> PUTable;
    std::vector<QString> Patterns;
    QMap<QString, int> totalNum;
    int currentPatternIndex;
};

#endif // STARTWORKING_H

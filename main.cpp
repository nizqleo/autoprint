#include "mainwindow.h"
#include <QApplication>

#include <QtGui>

#include <QTableView>
#include "models\ordermodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //数据源
//    QMap<QString, double> currencyMap;
//    currencyMap.insert("AUD", 1.3259);
//    currencyMap.insert("CHF", 1.2970);
//    currencyMap.insert("CZK", 24.510);
//    currencyMap.insert("DKK", 6.2168);
//    currencyMap.insert("EUR", 0.8333);
//    currencyMap.insert("GBP", 0.5661);
//    currencyMap.insert("HKD", 7.7562);
//    currencyMap.insert("JPY", 112.92);
//    currencyMap.insert("NOK", 6.5200);
//    currencyMap.insert("NZD", 1.4697);
//    currencyMap.insert("SEK", 7.8180);
//    currencyMap.insert("SGD", 1.6901);
//    currencyMap.insert("USD", 1.0000);

//    //自定义表模型
//    CurrencyModel currencyModel;
//    currencyModel.setCurrencyMap(currencyMap);
//    //表视图
//    QTableView tableView;
//    //设置视图模型
//    tableView.setModel(&currencyModel);
//    //设置交替颜色
//    tableView.setAlternatingRowColors(true);

//    tableView.show();
    return a.exec();
}

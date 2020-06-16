#include "printtaskmanagement.h"
#include "ui_printtaskmanagement.h"
#include "mainwindow.h"


printTaskManagement::printTaskManagement(MainWindow *mainwindow, QWidget *parent) :
    QWidget(parent),MW(mainwindow),
    ui(new Ui::printTaskManagement)
{
    ui->setupUi(this);
    api = MW->api;

    connect(this, SIGNAL(datapageopen()), MW, SLOT(open_datamaintainance()));
    connect(this, SIGNAL(settingpageopen()), MW, SLOT(open_printersetting()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_changed(const QString &)));
    connect(this, SIGNAL(createNewPattern(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            MW->DM, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
    QPixmap pixmap(".\\data\\logo.png");
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->show();

    printerNumber = MW->printers.size();
    ordermodels.resize(printerNumber);

    for (int i = 0; i < printerNumber;i++) {
        ui->comboBox->addItem(QString::fromStdString(to_string(MW->printers[i].printerID)+"号打印机"));
        ordermodels[i] = new OrderModel(i);
    }


    ui->tableView->setModel(ordermodels[0]);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->comboBox->setCurrentIndex(0);
    currentPrinterIndex= 0;
    ui->edit_button->setEnabled(false);
    ui->detail_button->setEnabled(false);
    ui->up_button->setEnabled(false);
    cout<<Task::DModel->getPatternSize()<<endl;
    on_ERP_button_clicked();
    ordermodels[0]->update();
}

printTaskManagement::~printTaskManagement()
{
    delete ui;
}


/* function -- orderAssignment
 *
 * Key operation for taskManagement page
 *
 * Take the orders, group them into tasks, assign the tasks
 * different printers
 *
 */
void printTaskManagement::orderAssignment(int begin, int end, int printerID){
    vector<int> assignment;
    assignment.resize( end - begin, 0);
    if(printerID == 0){
        // compute Assignment
        //
        //
        //
        // //////////////////
    }
    else{
        for(int i = 0; i < assignment.size(); i++)
            assignment[i] = printerID-1;
    }

    for(int i  = begin; i < end; i++){
        ordermodels[assignment[i-begin]]->addOrder(orderList[i]);
    }

    for(int i  = 0; i < ordermodels.size(); i++){
        ordermodels[i]->sortTable();
    }
}


void printTaskManagement::updateDatasetWithOrder(){
    int cnt = 0;
    for(int i = 0; i <orderList.size(); i++){
        if(!MW->DM->DModel->hasPatternName(orderList[i].pattern)){
             MW->DM->DModel->save_files(orderList[i].pattern, "","","","","","", true, true);
             cnt++;
        }
    }

    if(cnt > 0)
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "新款号", QString::fromStdString("本次订单中包含"+to_string(cnt) +"个新款号，已在数据库中建立条目，请及时完善相关文件。"), QMessageBox::Yes );
}


// swifting pattern picture distribution
void printTaskManagement::on_comboBox_changed(const QString & s){
    currentPrinterIndex = ui->comboBox->currentIndex();
    ui->tableView->setModel(
                ordermodels[currentPrinterIndex]);
    if(ordermodels[currentPrinterIndex])
        ui->num_label->setNum(ordermodels[currentPrinterIndex]->totalNum);
}

void printTaskManagement::on_dataPage_button_clicked(){
    emit datapageopen();
    this->close();
}

void printTaskManagement::on_settingPage_button_clicked(){
    emit settingpageopen();
    this->close();
}

void printTaskManagement::on_ERP_button_clicked(){
    // read order file
    // renew tableview
    api->getOrders(orderList);
    updateDatasetWithOrder();
    orderAssignment(0, orderList.size());
    ui->num_label->setNum(ordermodels[currentPrinterIndex]->totalNum);

}

void printTaskManagement::on_tableView_clicked(){
    ui->edit_button->setEnabled(true);
    ui->detail_button->setEnabled(true);
    //ui->down_button->setEnabled(true);
    ui->up_button->setEnabled(true);
}

void printTaskManagement::on_add_button_clicked(){
    addPrintTask * APT = new addPrintTask(MW);
    APT->show();
    connect(APT, SIGNAL(send_orders(int**, string, int)), this, SLOT(receive_orders(int**, string, int)));
}

void printTaskManagement::on_up_button_clicked(){
    int row = ui->tableView->currentIndex().row();
//    ordermodels[ui->comboBox->currentIndex()]->move(row, -1);
    ordermodels[currentPrinterIndex]->tasklist[row].topped =
            1-ordermodels[currentPrinterIndex]->tasklist[row].topped;
    updateTable();
}

void printTaskManagement::on_detail_button_clicked(){
    int row = ui->tableView->currentIndex().row();
    startWorking * SW = new startWorking(&ordermodels[currentPrinterIndex]->tasklist[row], SHOWING, row);
    SW->show();
    connect(SW, SIGNAL(asking_for_adjacent(int)), this, SLOT(receiving_asking(int)));
    connect(this, SIGNAL(sending_new_task(Task*, int)), SW, SLOT(receiving_new_task(Task*, int)));
}

void printTaskManagement::on_edit_button_clicked(){
    int row = ui->tableView->currentIndex().row();
    Dialog * dialog = new Dialog(MW->DM, MW->api);
    Task& tempTask = ordermodels[currentPrinterIndex]->tasklist[row];

    dialog->init(tempTask.pattern->name);
    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            MW->DM, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
    connect(dialog, SIGNAL(PatternNameChanged(QString)), MW->DM, SLOT(delete_files(QString)));
    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            this, SLOT(update(QString)));

    dialog->show();

}

void printTaskManagement::update(QString s){
    int row = ui->tableView->currentIndex().row();
    ordermodels[currentPrinterIndex]->update(row);
    ordermodels[currentPrinterIndex]->sortTable();
}


void printTaskManagement::receive_orders(int** numbers, string name, int printerID){

    int begin = orderList.size();
    for(int i = 0; i < 8; i++){
        for (int j =0; j < 5 ;j++) {
            if(numbers[i][j] != 0){
                orderList.push_back(Order(name, i, j, numbers[i][j]));
                orderList.rbegin()->fromERP = false;
            }
        }
    }

    int end = orderList.size();

    orderAssignment(begin, end, printerID);

    updateTable();
}

void printTaskManagement::receiving_asking(int row){
    if(row < 0 || row >= ordermodels[currentPrinterIndex]->tasklist.size()){
        emit sending_new_task(NULL, row);
    }
    else{
        emit sending_new_task(&ordermodels[currentPrinterIndex]->tasklist[row], row);
    }
    updateTable();
}

void printTaskManagement::on_startPrint_button_clicked(){
    int row = 0;
    startWorking * SW = new startWorking(&ordermodels[currentPrinterIndex]->tasklist[row], WORKING, row);
    SW->show();
    connect(SW, SIGNAL(asking_for_adjacent(int)), this, SLOT(receiving_asking(int)));
    connect(this, SIGNAL(sending_new_task(Task*, int)), SW, SLOT(receiving_new_task(Task*, int)));

    updateTable();
}


void printTaskManagement::updateTable(){
    ordermodels[currentPrinterIndex]->update();
//    ordermodels[currentPrinterIndex]->sortTable();
//    ui->tableView->setModel(ordermodels[currentPrinterIndex]);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

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

    QPixmap pixmap(".\\data\\logo.png");
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->show();

    printerNumber = MW->printers.size();
    ordermodels.resize(printerNumber);

    for (int i = 0; i < printerNumber;i++) {
        ui->comboBox->addItem(MW->printers[i].name);
        ordermodels[i] = new OrderModel(i);
        api->readSavedTasks(ordermodels[i]);
    }

    ui->tableView->setModel(ordermodels[0]);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ordermodels[0]->numberCheck();

    ui->num_label->setText(QString::number(ordermodels[0]->finishedNum) +" / " + QString::number(ordermodels[0]->totalNum));

    ui->comboBox->setCurrentIndex(0);
    currentPrinterIndex= 0;
    ui->edit_button->setEnabled(false);
    ui->detail_button->setEnabled(false);
    ui->up_button->setEnabled(false);
    cout<<Task::DModel->getPatternSize()<<endl;

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
        vector<int> LightPrinter;
        vector<int> DarkPrinter;
        for (int i = 0; i < printerNumber;i++) {
            if(MW->printers[i].availble && MW->printers[i].darkprint)
                DarkPrinter.push_back(i);
            if(MW->printers[i].availble && !MW->printers[i].darkprint)
                LightPrinter.push_back(i);
        }


        int ls = LightPrinter.size();
        int ds = DarkPrinter.size();
        int lp, dp; lp = dp = 0;
        for (int i = 0; i < end-begin; i++) {
            if(orderList[i+begin].isDark){
                assignment[i] = DarkPrinter[dp];
                dp++;
                if(dp == ds) dp = 0;
            }
            else{
                assignment[i] = LightPrinter[lp];
                lp++;
                if(lp == ls) lp = 0;
            }
        }
        // //////////////////
    }
    else{
        for(int i = 0; i < assignment.size(); i++)
            assignment[i] = printerID-1;
    }

    for(int i  = begin; i < end; i++){
        ordermodels[assignment[i-begin]]->addOrder(&orderList[i]);
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
        ui->num_label->setText(QString::number(ordermodels[currentPrinterIndex]->finishedNum) +" / "
                               + QString::number(ordermodels[currentPrinterIndex]->totalNum));
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
    int begin = orderList.size();
    api->getERPOrders(orderList);
    //api->saveOrders(orderList);

    updateDatasetWithOrder();
    orderAssignment(begin, orderList.size());
    ui->num_label->setText(QString::number(ordermodels[currentPrinterIndex]->finishedNum) +" / "
                           + QString::number(ordermodels[currentPrinterIndex]->totalNum));

    updateTable(currentPrinterIndex);

    for(int i = 0 ; i < ordermodels.size(); i++){
        api->saveTasks(ordermodels[i]);
    }
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
    connect(APT, SIGNAL(send_orders(int**, QString, int)), this, SLOT(receive_orders(int**, QString, int)));
}

void printTaskManagement::on_up_button_clicked(){
    if(!ui->tableView->currentIndex().isValid())
        return;
    int row = ui->tableView->currentIndex().row();
    if(ordermodels[currentPrinterIndex]->tasklist[row].Tstatus == PRINTING ||
            ordermodels[currentPrinterIndex]->tasklist[row].Tstatus == FINISHED){
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "置顶失败", "只能置顶尚未打印的任务！", QMessageBox::Yes);
        return;
    }
    if(ordermodels[currentPrinterIndex]->tasklist[row].topped > 0)
        ordermodels[currentPrinterIndex]->tasklist[row].topped = 0;
    else{
        ordermodels[currentPrinterIndex]->currentToppingNum ++;
        ordermodels[currentPrinterIndex]->tasklist[row].topped =
                ordermodels[currentPrinterIndex]->currentToppingNum;
    }

    updateTable(currentPrinterIndex);

    api->saveTasks(ordermodels[currentPrinterIndex]);
}

void printTaskManagement::on_detail_button_clicked(){
    if(!ui->tableView->currentIndex().isValid())
        return;
    int row = ui->tableView->currentIndex().row();
    startWorking * SW = new startWorking(&ordermodels[currentPrinterIndex]->tasklist[row],
                                         SHOWING, row, MW->printers[currentPrinterIndex].name);
    SW->show();
    connect(SW, SIGNAL(asking_for_adjacent(int, bool ,int)), this, SLOT(receiving_asking(int, bool , int)));
    connect(this, SIGNAL(sending_new_task(Task*, int, int)), SW, SLOT(receiving_new_task(Task*, int, int)));
}

void printTaskManagement::on_edit_button_clicked(){
    if(!ui->tableView->currentIndex().isValid())
        return;
    int row = ui->tableView->currentIndex().row();
    Dialog * dialog = new Dialog(MW->DM, MW->api);
    Task& tempTask = ordermodels[currentPrinterIndex]->tasklist[row];

    dialog->init(tempTask.pattern->name);
    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            MW->DM, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
    connect(dialog, SIGNAL(PatternNameChanged(QString)), MW->DM, SLOT(delete_files(QString)));
    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            this, SLOT(update(QString, QString, QString, QString, QString, QString, QString, bool,bool)));

    dialog->show();
}

void printTaskManagement::update(QString s, QString, QString, QString, QString, QString, QString, bool,bool){
    ordermodels[currentPrinterIndex]->update(s);
    ordermodels[currentPrinterIndex]->sortTable();
    ui->edit_button->setEnabled(false);
    ui->detail_button->setEnabled(false);
    ui->up_button->setEnabled(false);
}

void printTaskManagement::receive_orders(int** numbers, QString name, int printerID){

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

    for(int i = 0 ; i < ordermodels.size(); i++){
        updateTable(i);
        api->saveTasks(ordermodels[i]);
    }
}

/* Tasks are finished here.
 *
 */
void printTaskManagement::receiving_asking(int row, bool working, int ID){

    if(!working){
        if(row < 0 || row >= ordermodels[ID]->tasklist.size()){
            emit sending_new_task(NULL, row, ID);
        }
        else{
            emit sending_new_task(&ordermodels[ID]->tasklist[row], row, ID);
        }
    }
    else{
        if(row == -1){
            ordermodels[ID]->tasklist[0].Tstatus = SUSPENDED;
            updateTable(ID);
            api->saveTasks(ordermodels[ID]);
            emit sending_new_task(NULL, 0, ID);
            return;
        }

        topTaskFinished(ID);

        if(ordermodels[ID]->tasklist.size() == 1 || ordermodels[ID]->tasklist[1].Tstatus == FINISHED){
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "打印完成", "本打印机的打印任务已全部完成！", QMessageBox::Yes);
            emit sending_new_task(NULL, 0, ID);
            return;
        }

        ordermodels[ID]->tasklist[1].Tstatus = PRINTING;
        updateTable(ID);
        emit sending_new_task(&ordermodels[ID]->tasklist[0], 0, ID);
    }

}

void printTaskManagement::topTaskFinished(int printerID){
    ordermodels[printerID]->tasklist[0].Tstatus = FINISHED;
    ordermodels[printerID]->tasklist[0].topped = 0;
    api->saveTasks(ordermodels[printerID]);
    ordermodels[printerID]->tasklist[0].finishOrders();

    //api->saveOrders(orderList);
}

void printTaskManagement::on_startPrint_button_clicked(){
    int row = 0;
    if(ordermodels[currentPrinterIndex]->tasklist.empty())
        return;
    startWorking * SW = new startWorking(&ordermodels[currentPrinterIndex]->tasklist[row], WORKING, row, MW->printers[currentPrinterIndex].name);
    SW->show();

    connect(SW, SIGNAL(asking_for_adjacent(int, bool ,int)), this, SLOT(receiving_asking(int, bool , int)));
    connect(this, SIGNAL(sending_new_task(Task*, int, int)), SW, SLOT(receiving_new_task(Task*, int, int)));
    connect(SW, SIGNAL(update(int)), this, SLOT(updateTable(int)));

    ordermodels[currentPrinterIndex]->tasklist[row].Tstatus = PRINTING;
    updateTable(currentPrinterIndex);
}

void printTaskManagement::updateTable(int i){
    ordermodels[i]->update();
}

/* read orders from the orders.csv, which is the files that is being processed.
 *
 *
 */
void printTaskManagement::readSaveOrders(){
    // read order file
    // renew tableview
    api->readOrders(orderList);
    updateDatasetWithOrder();
    orderAssignment(0, orderList.size());
    ui->num_label->setText(QString::number(ordermodels[0]->finishedNum) +" / " + QString::number(ordermodels[0]->totalNum));


}

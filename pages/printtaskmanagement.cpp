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

    // prepare data and ordermodels for all printers.
    printerNumber = MW->printers.size();
    ordermodels.resize(printerNumber);

    for (int i = 0; i < printerNumber;i++) {
        QString comboBoxContent = MW->printers[i].name;
        if(MW->printers[i].darkprint)
            comboBoxContent += " (深色)";
        else comboBoxContent += " (浅色)";

        if(!MW->printers[i].availble)
            comboBoxContent += "(不可用)";

        ui->comboBox->addItem(comboBoxContent);

        ordermodels[i] = new OrderModel(i);
        api->readSavedTasks(ordermodels[i]);
        ordermodels[i]->numberCheck();
        updateTable(i);
    }

    // setting current showing printer data.
    ui->comboBox->setCurrentIndex(0);
    currentPrinterIndex= 0;

    ui->tableView->setModel(ordermodels[0]);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->num_label->setText(QString::number(ordermodels[0]->finishedNum) +" / " + QString::number(ordermodels[0]->totalNum));

    // setting default buttons
    ui->edit_button->setEnabled(false);
    ui->detail_button->setEnabled(false);
    ui->up_button->setEnabled(false);
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
 * printerID has default value 0
 *
 */
bool printTaskManagement::orderAssignment(int begin, int end, int printerID){

    bool haveMet = false;
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

        // if no dark printer, check if contains dark tasks
        if(ds == 0){
            for (int i = 0; i < end-begin; i++) {
                if(orderList[i+begin].isDark){
                    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "ERP任务导入失败", "ERP订单中包括深色打印任务，但未检测到深色打印机。ERP订单导入失败。是否继续导入并将深色订单分配到现有浅色打印机上？", QMessageBox::Yes |QMessageBox::No);
                    if(reply == QMessageBox::No){
                        return 0;
                    }
                    else {
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < end-begin; i++) {
            if(orderList[i+begin].isDark){
                if(ds == 0){
                    assignment[i] = LightPrinter[lp];
                    lp++;
                    if(lp == ls) lp = 0;
                }
                else{
                    assignment[i] = DarkPrinter[dp];
                    dp++;
                    if(dp == ds) dp = 0;
                }
            }
            else{
                assignment[i] = LightPrinter[lp];
                lp++;
                if(lp == ls) lp = 0;
            }
        }
        ////////////////////
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

    return 1;
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
    cout<<"comboBox changed"<<endl;
    currentPrinterIndex = ui->comboBox->currentIndex();
    ui->tableView->setModel(
                ordermodels[currentPrinterIndex]);
    if(ordermodels[currentPrinterIndex])
        ui->num_label->setText(QString::number(ordermodels[currentPrinterIndex]->finishedNum) +" / "
                               + QString::number(ordermodels[currentPrinterIndex]->totalNum));

    if(MW->printers[currentPrinterIndex].availble)
        ui->startPrint_button->setEnabled(true);
    else ui->startPrint_button->setEnabled(false);
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
    api->getERPOrders(orderList, this);

    updateDatasetWithOrder();
    qDebug()<<"updateDatasetWithOrder"<<endl;
    if(!orderAssignment(begin, orderList.size())){
        if(begin == 0)
            orderList.clear();
        else orderList.erase(orderList.begin()+begin, orderList.end());
        return;
    }

    qDebug()<<"orderAssignment"<<endl;
    ui->num_label->setText(QString::number(ordermodels[currentPrinterIndex]->finishedNum) +" / "
                           + QString::number(ordermodels[currentPrinterIndex]->totalNum));
    qDebug()<<"setText"<<endl;
    updateTable(currentPrinterIndex);
    qDebug()<<"updateTable"<<endl;
    for(int i = 0 ; i < ordermodels.size(); i++){
        api->saveTasks(ordermodels[i]);
    }
    qDebug()<<"saveTasks"<<endl;
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
    connect(APT, SIGNAL(send_orders(int**, QString, int, int)), this, SLOT(receive_orders(int**, QString, int, int)));
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

void printTaskManagement::receive_orders(int** numbers, QString name, int type, int printerID){

    int begin = orderList.size();
    for(int i = 0; i < 8; i++){
        for (int j =0; j < 5 ;j++) {
            if(numbers[i][j] != 0){
                orderList.push_back(Order(name, i, j, numbers[i][j], type));
                orderList.rbegin()->fromERP = false;
            }
        }
    }

    int end = orderList.size();

    if(!orderAssignment(begin, end, printerID)){
        if(begin == 0)
            orderList.clear();
        else orderList.erase(orderList.begin()+begin, orderList.end());
        return;
    }
    for(int i = 0 ; i < ordermodels.size(); i++){
        updateTable(i);
        api->saveTasks(ordermodels[i]);
    }
}

/* Tasks are finished here.
 *
 */
void printTaskManagement::receiving_asking(int row, bool working, int ID){
    cout<<"receiving_asking "<<ID<<endl;
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
            updateTable(ID);
            return;
        }
        if(ordermodels[ID]->tasklist[1].fileReady == false){
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "文件不全", "剩余打印任务文件不全，请补充对应打印文件后再打印。", QMessageBox::Yes);
            emit sending_new_task(NULL, 0, ID);
            updateTable(ID);
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
    ordermodels[printerID]->finishedNum += ordermodels[printerID]->tasklist[0].totalNum;
    if(currentPrinterIndex == printerID)
        ui->num_label->setText(QString::number(ordermodels[printerID]->finishedNum) +" / " + QString::number(ordermodels[printerID]->totalNum));
}

void printTaskManagement::on_startPrint_button_clicked(){
    cout<<"start printing, current Printer:" << currentPrinterIndex<<ordermodels[currentPrinterIndex]->tasklist[0].printerIdx<<endl;
    if(ordermodels[currentPrinterIndex]->tasklist.empty())
        return;

    if(ordermodels[currentPrinterIndex]->tasklist[0].fileReady == false){
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "文件不全", "剩余打印任务文件不全，请补充对应打印文件后再打印。", QMessageBox::Yes);
        return;
    }

    startWorking * SW = new startWorking(&ordermodels[currentPrinterIndex]->tasklist[0], WORKING, 0, MW->printers[currentPrinterIndex].name);
    SW->show();

    connect(SW, SIGNAL(asking_for_adjacent(int, bool ,int)), this, SLOT(receiving_asking(int, bool , int)));
    connect(this, SIGNAL(sending_new_task(Task*, int, int)), SW, SLOT(receiving_new_task(Task*, int, int)));
    connect(SW, SIGNAL(update(int)), this, SLOT(updateTable(int)));

    ordermodels[currentPrinterIndex]->tasklist[0].Tstatus = PRINTING;
    updateTable(currentPrinterIndex);
}

void printTaskManagement::updateTable(int i){
    ordermodels[i]->update();
}


void printTaskManagement::totalUpdate(){
    for(unsigned long long i = 0; i < ordermodels.size(); i++)
        ordermodels[i]->update();
}

//* read orders from the orders.csv, which is the files that is being processed.
// *
// *
// */
//void printTaskManagement::readSaveOrders(){
//    // read order file
//    // renew tableview
//    api->readOrders(orderList);
//    updateDatasetWithOrder();
//    orderAssignment(0, orderList.size());
//    ui->num_label->setText(QString::number(ordermodels[0]->finishedNum) +" / " + QString::number(ordermodels[0]->totalNum));
//}

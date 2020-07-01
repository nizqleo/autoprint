#include "printtaskmanagement.h"
#include "ui_printtaskmanagement.h"
#include "mainwindow.h"
#include "API/api.h"

printTaskManagement::printTaskManagement(MainWindow *mainwindow, QWidget *parent) :
    QWidget(parent),MW(mainwindow),
    ui(new Ui::printTaskManagement)
{
    ui->setupUi(this);


    connect(this, SIGNAL(datapageopen()), MW, SLOT(open_datamaintainance()));
    connect(this, SIGNAL(settingpageopen()), MW, SLOT(open_printersetting()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(comboBox_changed(const QString &)));
    connect(ui->type_comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(type_comboBox_changed(const QString &)));
    connect(this, SIGNAL(createNewPattern(QString, int)), MW->DM, SLOT(AddNewPattern(QString, int)));

//    QPixmap pixmap(".\\data\\logo.png");
//    ui->logoLabel->setPixmap(pixmap);
//    ui->logoLabel->show();

    // prepare data and ordermodels for all printers.
    printerNumber = MW->printers.size();
    ordermodels.resize(printerNumber);

    LeftNumbers = new int*[colorString.size()];
    for(int i = 0; i < colorString.size(); i++){
        LeftNumbers[i] = new int[6];
        for(int j = 0; j < 6; j++){
            LeftNumbers[i][j] = 0;
        }
    }

    for (int i = 0; i < printerNumber;i++) {
        connect(this, SIGNAL(filterType(int ,int )), ordermodels[i], SLOT(filterType(int ,int)));
        QString comboBoxContent = MW->printers[i].name;
        if(MW->printers[i].darkprint)
            comboBoxContent += " (深色)";
        else comboBoxContent += " (浅色)";

        if(!MW->printers[i].availble)
            comboBoxContent += "(不可用)";

        ui->comboBox->addItem(comboBoxContent);

        ordermodels[i] = new OrderModel(i);
        api->readSavedTasks(ordermodels[i], LeftNumbers);

        ordermodels[i]->numberCheck();

        updateTable(i);
    }
//    for (int i = 0; i < colorString.size(); ++i){
//        for (int j = 0; j < 6; ++j){
//            qDebug()<<LeftNumbers[i][j]<<" ";
//        }qDebug()<<endl;
//    }
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

    ui->type_comboBox->addItem(QString("所有"));
    for(int i = 0; i < 4; i++){
        ui->type_comboBox->addItem(printTypeString[i]);
    }
    ui->type_comboBox->setCurrentIndex(0);

    TaskLeftModel = NULL;
    PrinterModel = NULL;

    computeDashBoardModels();

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
        LeftNumbers[orderList[i].color][orderList[i].size] += orderList[i].number;
        LeftNumbers[orderList[i].color][5] += orderList[i].number;
    }

    for(int i  = 0; i < ordermodels.size(); i++){
        ordermodels[i]->sortTable();
    }

    computeDashBoardModels();

    return 1;
}


void printTaskManagement::updateDatasetWithOrder(){
    int cnt = 0;
    for(int i = 0; i <orderList.size(); i++){
        if(!MW->DM->DModel->hasPatternName(orderList[i].pattern)){
             vector<QString> temp;
             api->savePatternData(orderList[i].pattern, "", "", orderList[i].type, temp, NULL);
             emit createNewPattern(orderList[i].pattern, orderList[i].type);
             cnt++;
        }
    }

    if(cnt > 0)
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "新款号", QString::fromStdString("本次订单中包含"+to_string(cnt) +"个新款号，已在数据库中建立条目，请及时完善相关文件。"), QMessageBox::Yes );
}

// swifting pattern picture distribution
void printTaskManagement::comboBox_changed(const QString & s){
    //cout<<"comboBox changed"<<endl;
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
    try {
        // read order file
        // renew tableview
        int begin = orderList.size();
        api->getERPOrders(orderList, this);

        updateDatasetWithOrder();
        if(!orderAssignment(begin, orderList.size())){
            if(begin == 0)
                orderList.clear();
            else orderList.erase(orderList.begin()+begin, orderList.end());
            return;
        }

        ui->num_label->setText(QString::number(ordermodels[currentPrinterIndex]->finishedNum) +" / "
                               + QString::number(ordermodels[currentPrinterIndex]->totalNum));

        updateTable(currentPrinterIndex);
        for(int i = 0 ; i < ordermodels.size(); i++){
            api->saveTasks(ordermodels[i]);
        }
    } catch (exception& e) {
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "发生错误", "e.what()", QMessageBox::Yes );
    }
}

void printTaskManagement::on_tableView_clicked(){
    ui->edit_button->setEnabled(true);
    ui->detail_button->setEnabled(true);
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
    startWorking * SW = new startWorking(ordermodels[currentPrinterIndex]->getTaskAtIndex(row),
                                         SHOWING, row, MW->printers[currentPrinterIndex].name);
    SW->show();
    connect(SW, SIGNAL(asking_for_adjacent(int, bool ,int)), this, SLOT(receiving_asking(int, bool , int)));
    connect(this, SIGNAL(sending_new_task(Task*, int, int)), SW, SLOT(receiving_new_task(Task*, int, int)));
}

void printTaskManagement::on_edit_button_clicked(){
    if(!ui->tableView->currentIndex().isValid())
        return;
    int row = ui->tableView->currentIndex().row();
    Task& tempTask = *ordermodels[currentPrinterIndex]->getTaskAtIndex(row);
    Dialog * dialog = new Dialog(MW->DM, tempTask.pattern);


    //dialog->init(tempTask.pattern);
    connect(dialog, SIGNAL(confirmEditing(Pattern*)),MW->DM, SLOT(AddNewPattern(Pattern*)));
    connect(dialog, SIGNAL(PatternNameChanged(QString)), MW->DM, SLOT(delete_files(QString)));
    connect(dialog, SIGNAL(confirmEditing(Pattern*)),
            this, SLOT(updatePatternInfo(Pattern*)));

    dialog->show();
}

void printTaskManagement::updatePatternInfo(Pattern* p){
    QString s = p->name;
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
    if(!working){
        if(row < 0 || ordermodels[ID]->getTaskAtIndex(row) == NULL){
            emit sending_new_task(NULL, row, ID);
        }
        else{
            emit sending_new_task(ordermodels[ID]->getTaskAtIndex(row), row, ID);
        }
    }
    else{
        if(row == -1){
            ordermodels[ID]->getTaskAtIndex(0)->Tstatus = SUSPENDED;
            updateTable(ID);
            api->saveTasks(ordermodels[ID]);
            emit sending_new_task(NULL, 0, ID);
            return;
        }

        topTaskFinished(ID);
        if(ordermodels[ID]->getTaskAtIndex(1) == NULL || ordermodels[ID]->getTaskAtIndex(1)->Tstatus == FINISHED){
            if(ui->type_comboBox->currentIndex() == 0)
                QMessageBox::information(NULL, "打印完成", "本打印机的打印任务已全部完成！", QMessageBox::Yes);
            else
                QMessageBox::information(NULL, "打印完成", "当前类型的打印任务已全部完成，切换类型查看更多任务。", QMessageBox::Yes);
            emit sending_new_task(NULL, 0, ID);
            updateTable(ID);
            return;
        }

        ordermodels[ID]->getTaskAtIndex(1)->Tstatus = PRINTING;
        updateTable(ID);
        emit sending_new_task(ordermodels[ID]->getTaskAtIndex(0), 0, ID);
    }
}

void printTaskManagement::topTaskFinished(unsigned long long printerID){
    Task* task = ordermodels[printerID]->getTaskAtIndex(0);
    task->Tstatus = FINISHED;
    task->topped = 0;
    api->saveTasks(ordermodels[printerID]);
    task->finishOrders();

    ordermodels[printerID]->finishedNum += task->totalNum;
    if(currentPrinterIndex == printerID)
        ui->num_label->setText(QString::number(ordermodels[printerID]->finishedNum) +" / " + QString::number(ordermodels[printerID]->totalNum));

    for(int i = 0; i < 5; i++){
        LeftNumbers[task->colorInfo][i] -= task->numbers[i];


        assert(LeftNumbers[task->colorInfo][i] >= 0);
    }
    LeftNumbers[task->colorInfo][5] -= task->totalNum;
    assert(LeftNumbers[task->colorInfo][5] >= 0);

    computeDashBoardModels();
}

void printTaskManagement::on_startPrint_button_clicked(){
    cout<<"start printing, current Printer:" << currentPrinterIndex<<ordermodels[currentPrinterIndex]->getTaskAtIndex(0)->printerIdx<<endl;
    if(ordermodels[currentPrinterIndex]->tasklist.empty())
        return;

    startWorking * SW = new startWorking(ordermodels[currentPrinterIndex]->getTaskAtIndex(0), WORKING, 0, MW->printers[currentPrinterIndex].name);
    SW->show();

    connect(SW, SIGNAL(asking_for_adjacent(int, bool ,int)), this, SLOT(receiving_asking(int, bool , int)));
    connect(this, SIGNAL(sending_new_task(Task*, int, int)), SW, SLOT(receiving_new_task(Task*, int, int)));
    connect(SW, SIGNAL(updatePrinterTaskStatus(int)), this, SLOT(updateTable(int)));

    ordermodels[currentPrinterIndex]->getTaskAtIndex(0)->Tstatus = PRINTING;
    updateTable(currentPrinterIndex);
}

void printTaskManagement::updateTable(int i){
    ordermodels[i]->update();
}

void printTaskManagement::totalUpdate(){
    for(unsigned long long i = 0; i < ordermodels.size(); i++)
        ordermodels[i]->update();
}

void printTaskManagement::type_comboBox_changed(const QString & s){
    ordermodels[ui->comboBox->currentIndex()]->filter( ui->type_comboBox->currentIndex()-1);
    ui->edit_button->setEnabled(false);
    ui->detail_button->setEnabled(false);
}

void printTaskManagement::on_dashboard_botton_clicked(){
    DB = new DashBoard(this);
    DB->show();
    connect(this, SIGNAL(DBmodelsReady()), DB, SLOT(updateTableViews()));
}
void printTaskManagement::on_yesterday_botton_clicked(){

}

int printTaskManagement::totalUnfinishedNumber(int color, int size){
    return 1;
}


void printTaskManagement::computeDashBoardModels(){
    if(TaskLeftModel) delete TaskLeftModel;

    QStringList labels;

    int colorNums = 0;
    vector<int> colorIndexs;
    for(int i = 0; i < colorString.size(); i++){
        if(LeftNumbers[i][5] > 0){
            colorNums++;
            labels<<colorString[i];
            colorIndexs.push_back(i);
        }
    }
    TaskLeftModel = new QStandardItemModel(colorNums, 6);
    TaskLeftModel->setVerticalHeaderLabels(labels);

    labels.clear();
    labels = QObject::trUtf8("S,M,L,XL,XXL,总计").simplified().split(",");
    TaskLeftModel->setHorizontalHeaderLabels(labels);

    for (int row = 0; row < colorNums; ++row){
        for (int column = 0; column < 6; ++column){
            QModelIndex index = TaskLeftModel->index(row, column, QModelIndex());
            TaskLeftModel->setData(index, QVariant(LeftNumbers[colorIndexs[row]][column]));
        }
    }



    if(PrinterModel) delete PrinterModel;
    PrinterModel = new QStandardItemModel(printerNumber, 3);

    labels.clear();
    labels = QObject::trUtf8("打印机编号,总计剩余件数,已完成件数").simplified().split(",");
    PrinterModel->setHorizontalHeaderLabels(labels);

    for (int row = 0; row < printerNumber; ++row){

        QModelIndex index = PrinterModel->index(row, 0, QModelIndex());
        PrinterModel->setData(index, QVariant(MW->printers[row].name));

        index = PrinterModel->index(row, 1, QModelIndex());
        PrinterModel->setData(index, QVariant(ordermodels[row]->totalNum-ordermodels[row]->finishedNum));

        index = PrinterModel->index(row, 2, QModelIndex());
        PrinterModel->setData(index, QVariant(ordermodels[row]->finishedNum));
    }

    emit DBmodelsReady();
}




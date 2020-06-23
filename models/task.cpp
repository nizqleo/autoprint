#include "task.h"
#include "datasetmodel.h"

Task::Task()
{
    init();
}

Task::~Task(){
    //delete pattern;
}

Task::Task( Order* o){
    init();

    //cout<<o->pattern<<endl;
    name = o->pattern;

    pattern = DModel->patternPointer(o->pattern);

    numbers[o->size] = o->number;
    totalNum = o->number;
    pattern->computeFileReady();

    isDark = o->isDark;
    fileReady = ((isDark && pattern->DarkReady) || (!isDark && pattern->LightReady));

    type = o->type;

    QDateTime curDateTime=QDateTime::currentDateTime();
    createTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    fromERP = o->fromERP;
    colorInfo = o->color;

    orders.push_back(o);

}

Task::Task( vector<QString>& temp, int ID):printerIdx(ID){
    //cout<<o->pattern<<endl;
    name = temp[0];

    pattern = DModel->patternPointer(temp[0]);
    type = Order::type2index(temp[1]);
    totalNum = temp[2].toInt();
    colorInfo = Order::color2Index(temp[3]);
    fromERP = temp[4].toInt();
    createTime = temp[5];
    Tstatus = TaskStatus(temp[6].toInt());
    topped = temp[8].toInt();
    sizeNumberStringAnalysis(temp[9]);

    pattern->computeFileReady();
    if(colorInfo == 1 )
        isDark = true;
    else isDark = false;
    fileReady = ((isDark && pattern->DarkReady) || (!isDark && pattern->LightReady));

}

void Task::init(){

    for(int j = 0; j < 6; j++){
        numbers[j] = 0;
    }


    Tstatus = PENDING;
    fileReady = false;
    fromERP = false;
    topped = false;
}

void Task::update(){
    if(pattern)
        pattern->computeFileReady();
    fileReady = ((isDark && pattern->DarkReady) || (!isDark && pattern->LightReady));
}
datasetModel* Task::DModel;


void Task::Add(Order* o){
    numbers[o->size]+=o->number;
    totalNum += o->number;
    orders.push_back(o);
}

/* sorting funciton.
 *
 * There are several criteria, listed in priority:
 *
 * task status: printing first, pending second, finished last
 * hand topping : always put the ones with hand toppings first
 * fileReady: always show the tasks with no file ready first
 * color: following the
 * createTime : the earlier the faster
 * totalNum: print the task with more cloth first
 */
bool Task::operator<(const Task & right)const   //重载<运算符
{
    // task status
    if(Tstatus == PRINTING){
        if(right.Tstatus == FINISHED||right.Tstatus == PENDING ||
                right.Tstatus == SUSPENDED) return true;
    }
    else if(Tstatus == FINISHED){
        if(right.Tstatus == PRINTING||right.Tstatus == PENDING|| right.Tstatus == SUSPENDED)
            return false;
    }
    else if(Tstatus == SUSPENDED){
        if(right.Tstatus == PRINTING) return false;
        if(right.Tstatus == FINISHED) return true;
        if(topped > right.topped)   return true;
        if(topped < right.topped)   return false;

        if(right.Tstatus == PENDING){
            // hand topping
            if(!topped && right.topped) return false;
            else return true;
        }
    }
    else if(Tstatus == PENDING){
        if(right.Tstatus == FINISHED ) return true;
        if(right.Tstatus == PRINTING) return false;
        if(topped > right.topped)   return true;
        if(topped < right.topped)   return false;

        if(right.Tstatus == SUSPENDED){
            // hand topping
            if(topped && !right.topped) return true;
            else return false;
        }
    }


    // file ready
    if(!fileReady && right.fileReady) return false;
    if(fileReady && !right.fileReady) return true;


    // hand topping
    if(topped && !right.topped) return true;
    if(!topped && right.topped) return false;


    // color
    if(colorInfo < right.colorInfo) return true;
    if(colorInfo > right.colorInfo) return false;

    // createTime


    // totalNum
    if(totalNum < right.totalNum) return true;
    if(totalNum > right.totalNum) return false;

    return false;
}

void Task::finishOrders(){
    for(int i =0; i < orders.size(); i++){
        orders[i]->finished = true;
    }
}


QString Task::sizeNumberString(){
    return QString::number(numbers[0])+"-"+
            QString::number(numbers[1])+"-"+
            QString::number(numbers[2])+"-"+
            QString::number(numbers[3])+"-"+
            QString::number(numbers[4]);
}

void Task::sizeNumberStringAnalysis(QString s){
    vector<QString> temp;
    SplitString(s, temp, "-");
    for(int i = 0; i < 5; i++)
        numbers[i] = temp[i].toInt();
}

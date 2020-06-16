#include "task.h"
#include "datasetmodel.h"

Task::Task()
{
    init();
}

Task::~Task(){
    //delete pattern;
}

Task::Task(const Order o){
    init();

    cout<<o.pattern<<endl;
    name = o.pattern;

    pattern = DModel->patternPointer(o.pattern);

    numbers[o.color][o.size] = o.number;
    totalNum = o.number;
    pattern->computeFileReady();

    isDark = o.isDark;
    fileReady = ((isDark && pattern->DarkReady) || (!isDark && pattern->LightReady));

    type = o.type;

    QDateTime curDateTime=QDateTime::currentDateTime();
    createTime = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    fromERP = o.fromERP;
    colorInfo = o.color;

    cout<<"finished."<<endl;
}


void Task::init(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 6; j++){
            numbers[i][j] = 0;
        }
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


void Task::Add(Order o){
    numbers[o.color][o.size]+=o.number;
    totalNum += o.number;
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
        if(right.Tstatus == FINISHED||right.Tstatus == PENDING) return true;
    }
    else if(Tstatus == PENDING){
        if(right.Tstatus == FINISHED ) return true;
        else if(right.Tstatus == PRINTING ) return false;
    }
    else if(Tstatus == FINISHED)
        if(right.Tstatus == PRINTING||right.Tstatus == PENDING) return false;

    // file ready
    if(!fileReady && right.fileReady) return true;
    if(fileReady && !right.fileReady) return false;


    // hand topping
    if(topped && !right.topped) return true;
    if(!topped && right.topped) return false;


    // color
    if(colorInfo < right.colorInfo) return true;
    if(colorInfo > right.colorInfo) return false;

    // createTime


    // totalNum
    if(totalNum > right.totalNum) return true;
    if(totalNum < right.totalNum) return false;

    return false;
}

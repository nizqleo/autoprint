#include "api.h"
#include "mainwindow.h"

API::API()
{

}



API::API(MainWindow* MW):MW(MW)
{

}




const  QString API::LFPrefix = "浅-前-";
const  QString API::LBPrefix = "浅-后-";
const  QString API::DFPrefix = "深-前-";
const  QString API::DBPrefix = "深-后-";

const  QString API::PimName = "生产版单";
const  QString API::MimName = "效果图";



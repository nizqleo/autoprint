#include "printer.h"

Printer::Printer(QString info, int ID):printerID(ID)
{
    load(info);
}



void Printer::load(QString info){

    vector<QString> infos;
    SplitString(info, infos, ",");
    name = infos[0];
    availble = infos[1].toInt();
    darkprint = infos[2].toInt();
    pro = infos[3].toInt();
}

string Printer::save(){
    string ID = to_string(printerID);
    string avai = to_string(availble);
    string dark = to_string(darkprint);

    return ID+' '+avai+' '+dark;
}

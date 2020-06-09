#include "printer.h"

printer::printer(string info)
{
    load(info);
}



void printer::load(string info){
    vector<string> infos;
    SplitString(info, infos, ",");
    printerID = atoi(infos[0].c_str());
    availble =  atoi(infos[1].c_str());
    darkprint =  atoi(infos[2].c_str());
}

string printer::save(){
    string ID = to_string(printerID);
    string avai = to_string(availble);
    string dark = to_string(darkprint);

    return ID+' '+avai+' '+dark;
}


#include "auxiliary.h"

int get_files(string fileFolderPath,std::vector<string>&file)
{
    string fileFolder = fileFolderPath + "\\*";// + fileExtension;
    string fileName;
    struct _finddata_t fileInfo;

    long long findResult = _findfirst(fileFolder.c_str(), &fileInfo);
    if (findResult == -1)
    {
        _findclose(findResult);
        return 0;
    }

    do
    {
        fileName = fileFolderPath + "\\" + fileInfo.name;
        if (fileInfo.attrib == _A_SUBDIR && fileInfo.name[0] != '.')
        {
            file.push_back(string(fileInfo.name));
        }
    } while (_findnext(findResult, &fileInfo) == 0);

    _findclose(findResult);
}

int SizeMap(string size){
    if(size == "S")
        return 0;
    if(size == "M")
        return 1;
    if(size == "L")
        return 2;
    if(size == "XL")
        return 3;
    if(size == "XXL")
        return 4;
}

int ColorMap(string size){
    if(size == "W")
        return 0;
    if(size == "B")
        return 1;
    if(size == "P")
        return 2;
    if(size == "B")
        return 3;
    if(size == "G")
        return 4;
    if(size == "Y")
        return 5;
    if(size == "U")
        return 6;
    if(size == "R")
        return 7;
}

void SplitString(const string& s, std::vector<string>& v, const string& c)
{
  string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

int coverFileCopy(string targetPath, QString originalPath){
    QString QtargetPath = QString::fromStdString(targetPath);
    if(QtargetPath == originalPath){
        return 1;
    }
    QFileInfo fi(QtargetPath);
    if(fi.exists()){
        std::remove(QtargetPath.toStdString().data());
    }
    if(QFile::copy(originalPath,QtargetPath)){
        return 1;
    }
    else{
        return 0;
    }
}

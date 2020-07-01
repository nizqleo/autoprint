#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <vector>
#include <string>
#include <io.h>
#include <iostream>
#include <QFileInfo>
#include <QEventLoop>
#include <QTimer>
#include <codecvt>
#include <locale>
#include <QDir>
#include <QDebug>
#include <QtXml/QDomDocument>


using namespace std;

int get_files(QString fileFolderPath, vector<QString>&file, bool isDir);

int SizeMap(string size);

int ColorMap(string size);

void SplitString(const string& s, vector<string>& v, const string& c);
void SplitString(const QString& s, vector<QString>& v, const QString& c);

int coverFileCopy(QString targetPath, QString originalPath);

bool checkSameFile(QString targetPath, QString referencePath);

void Delay_MSec(unsigned int msec);

std::string UnicodeToUTF8(const std::wstring & wstr);

std::wstring s2ws(const std::string& s);

QString readXml(const QString& path, const QString& keynode);

bool deleteDirectory(const QString &path);


#endif // AUXILIARY_H

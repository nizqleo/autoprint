#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <vector>
#include <string>
#include <io.h>
#include <iostream>
#include <QString>
#include <QFileInfo>
#include <QEventLoop>
#include <QTimer>
#include <codecvt>
#include <locale>

using namespace std;

int get_files(string fileFolderPath, vector<string>&file);

int SizeMap(string size);

int ColorMap(string size);

void SplitString(const string& s, vector<string>& v, const string& c);

int coverFileCopy(string targetPath, QString originalPath);

bool checkSameFile(QString targetPath, QString referencePath);

void Delay_MSec(unsigned int msec);

std::string UnicodeToUTF8(const std::wstring & wstr);

std::wstring s2ws(const std::string& s);
#endif // AUXILIARY_H

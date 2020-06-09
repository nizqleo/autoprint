#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <vector>
#include <string>
#include <io.h>
#include <iostream>
#include <QString>
#include <QFileInfo>

using namespace std;

int get_files(string fileFolderPath, vector<string>&file);

int SizeMap(string size);

int ColorMap(string size);

void SplitString(const string& s, vector<string>& v, const string& c);

int coverFileCopy(string targetPath, QString originalPath);

#endif // AUXILIARY_H

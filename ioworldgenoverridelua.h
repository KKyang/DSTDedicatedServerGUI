#ifndef IOWORLDGENOVERRIDELUA_H
#define IOWORLDGENOVERRIDELUA_H

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "dstdatastructure.h"

class IOWorldGenOverrideLua
{
public:
    IOWorldGenOverrideLua();
    bool readLuaFile(std::vector<properties> &PV, const QString &fileName);
    bool writeLuaFile(std::vector<properties> &PV, const QString &fileName);
};

#endif // IOWORLDGENOVERRIDELUA_H

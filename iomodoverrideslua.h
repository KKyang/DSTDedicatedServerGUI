#ifndef IOMODOVERRIDESLUA_H
#define IOMODOVERRIDESLUA_H
#include <QObject>
#include <QDir>
#include "dstdatastructure.h"

class IOModOverridesLua
{
public:
    IOModOverridesLua();
    bool readInstalledLuaFile(std::vector<mods_properties> &mods, const QString &rootDirectory);
    bool readLuaFile(std::vector<mods_properties> &mods, const QString &fileName);
    bool writeLuaFile(std::vector<mods_properties> &mods, const QString &fileName);
signals:

public slots:
};

#endif // IOMODOVERRIDESLUA_H

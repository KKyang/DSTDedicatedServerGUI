#include "ioworldgenoverridelua.h"

IOWorldGenOverrideLua::IOWorldGenOverrideLua()
{

}

//Thanks @jefftsai8049
bool IOWorldGenOverrideLua::readLuaFile(std::vector<properties> &PV, const QString &fileName)
{
    PV.clear();

    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly);
            int lineCount = 0;
            while(!file.atEnd())
            {
                QString msg;
                msg = file.readLine();
                msg = msg.trimmed();
                if(msg.indexOf("override_enabled = true,") > -1)
                {
                    while(!file.atEnd())
                    {
                        QString data = file.readLine().trimmed();
                        QString segment = data.split("--")[0];
                        if(segment.indexOf('{') > -1)
                        {
                            properties P;
                            P.group = segment.split("--")[0].split('=')[0].replace(" ","");
                            while(!file.atEnd())
                            {

                                QString inGroup = file.readLine().trimmed();
                                inGroup = inGroup.split("--")[0];
                                if(inGroup.indexOf('}') > -1)
                                {
                                    break;
                                }
                                else
                                {
                                    QStringList inGroupData = inGroup.split('=');
                                    P.name = inGroupData[0].replace(" ","");
                                    P.settings = inGroupData[1].replace(" ","").replace('"',"").replace(',',"");
                                    PV.push_back(P);
                                }
                            }
                        }
                    }
                }
                lineCount++;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool IOWorldGenOverrideLua::writeLuaFile(std::vector<properties> &PV, const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite))
    {
       return false;
    }
    QTextStream stream(&file);

    stream << "return{\n\n";
    stream << " override_enabled = true,\n";

    for(int i = 0; i < PV.size(); i++)
    {

    }

    file.close();
}

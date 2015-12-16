#include "ioworldgenoverridelua.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif

IOWorldGenOverrideLua::IOWorldGenOverrideLua()
{

}

//Thanks @jefftsai8049
bool IOWorldGenOverrideLua::readLuaFile(std::vector<properties> &PV, std::vector<std::vector<QString>> &IV, const QString &fileName)
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

                                QString inGroup1 = file.readLine().trimmed();
                                QString inGroup = inGroup1.split("--")[0];
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

                                    std::vector<QString> select;
                                    if(inGroup1.indexOf(QString("-- ")) > 0)
                                    {
                                        inGroup1 = inGroup1.split("-- ")[inGroup1.split("-- ").size() - 1];
#ifdef QT_DEBUG
                                        qDebug() << inGroup1;
#endif
                                        inGroup1 = inGroup1.split(":")[inGroup1.split(":").size() - 1];
#ifdef QT_DEBUG
                                        qDebug() << inGroup1;
#endif
                                        inGroupData = inGroup1.split(",");
                                        for(int i = 0; i < inGroupData.size(); i++)
                                        {
                                            select.push_back(inGroupData[i].replace(" ","").replace('"',""));
                                        }
                                    }
                                    else
                                    {
                                        select.push_back("never");
                                        select.push_back("rare");
                                        select.push_back("default");
                                        select.push_back("often");
                                        select.push_back("always");
                                    }
                                    IV.push_back(select);
#ifdef QT_DEBUG
                                    QString show = "-- ";
                                    for(int i = 0; i < select.size(); i++)
                                    {
                                        show = show + QString(" ") + select[i];
                                    }
                                    qDebug() << P.group << " " << P.name << " " << P.settings << " " << show;
#endif
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

bool IOWorldGenOverrideLua::writeLuaFile(std::vector<properties> &PV, std::vector<std::vector<QString>> &IV, const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite))
    {
       return false;
    }
    QTextStream stream(&file);

    stream << "return{\n\n";
    stream << " override_enabled = true,\n";

    QString last_group_name;
    for(int i = 0; i < PV.size(); i++)
    {
        properties &p = PV[i];
        std::vector<QString> &ss = IV[i];
        if(p.group.compare(last_group_name))
        {
            if(!last_group_name.isEmpty())
            {
                stream << "       },\n";
            }
            if(p.group.compare("misc"))
            {
                stream << "       " << p.group << " = { -- ";
                QString show;
                for(int j = 0; j < ss.size(); j++)
                {
                    show = show + QString('"') + ss[j] + QString('"');
                    if(j == ss.size() - 1)
                    {
                        break;
                    }
                    show = show + QString(", ");
                }
                stream << show << "\n";
            }
            else
            {
                stream << "       " << p.group << " = {\n";
            }
        }

        if(p.group.compare("misc"))
        {
            stream << "               " << p.name << " = \"" << p.settings << "\",\n";
        }
        else
        {
            stream << "               " << p.name << " = \"" << p.settings << "\", -- ";
            QString show;
            for(int j = 0; j < ss.size(); j++)
            {
                show = show + QString('"') + ss[j] + QString('"');
                if(j == ss.size() - 1)
                {
                    break;
                }
                show = show + QString(", ");
            }
            stream << show << "\n";
        }

        last_group_name = p.group;
    }
    stream << "       },\n";
    stream << "}";

    file.close();
}

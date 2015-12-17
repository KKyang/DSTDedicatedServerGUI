#include "iomodoverrideslua.h"
#include <QDebug>

IOModOverridesLua::IOModOverridesLua()
{

}

bool IOModOverridesLua::readInstalledLuaFile(std::vector<mods_properties> &mods, const QString &rootDirectory)
{
    mods.clear();
    QDir findDir(rootDirectory);
    QStringList name_filters = {"workshop-*"};
    QStringList mod_folders;
    mod_folders = findDir.entryList(name_filters, QDir::Dirs);
#ifdef QT_DEBUG
    qDebug () << mod_folders;
#endif

    for(int num = 0; num < mod_folders.size(); num++)
    {
        QString fileName = rootDirectory + QString("/") + mod_folders[num] + QString("/modinfo.lua");

#ifdef QT_DEBUG
    qDebug () << fileName;
#endif

        if(!fileName.isEmpty())
        {
            QFile file(fileName);
            if(file.exists())
            {
                mods_properties mmp;
                mmp.client_only_mod = false;
                mmp.isEnabled = false;
                mmp.id = mod_folders[num].split("-")[1];
                file.open(QIODevice::ReadOnly);
                while(!file.atEnd())
                {
                    QString msg;
                    msg = file.readLine();
                    msg = msg.trimmed();
                    QString msg1 = msg.split("--")[0];


                    if(msg1.toLower().indexOf("name") > -1)
                    {
                        mmp.name = msg1.split("=")[1].replace(" \"","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("description") > -1)
                    {
                        mmp.description = msg1.split("=")[1].replace(" \"","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("author") > -1)
                    {
                        mmp.author = msg1.split("=")[1].replace(" \"","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("api_version") > -1)
                    {
                        mmp.api_version = msg1.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("version") > -1)
                    {
                        mmp.version = msg1.split("=")[1].replace(" \"","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("forumthread") > -1)
                    {
                        mmp.forumthread = msg1.split("=")[1].replace(" \"","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("icon_atlas") > -1)
                    {
                        mmp.icon_atlas = msg1.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("icon") > -1)
                    {
                        mmp.icon = msg1.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("dst_compatible") > -1)
                    {
                        msg1.split("=")[1].replace(" ","").replace('"',"").compare("true") ? mmp.dst_compatible = false : mmp.dst_compatible = true;
                    }
                    else if(msg1.toLower().indexOf("client_only_mod") > -1)
                    {
                        if(!msg1.split("=")[1].replace(" ","").replace('"',"").compare("true")) {break;}
                        else{mmp.client_only_mod = false;}
                    }
                    else if(msg1.toLower().indexOf("all_clients_require_mod") > -1)
                    {
                        msg1.split("=")[1].replace(" ","").replace('"',"").compare("true") ? mmp.all_clients_require_mod = false : mmp.all_clients_require_mod = true;
                    }
                    else if(msg1.toLower().indexOf("configuration_options") > -1)
                    {
                        QString data = file.readLine().trimmed();
                        QString segment = data.split("--")[0];
                        if(segment.indexOf("{") > - 1)
                        {
                            while(!file.atEnd())
                            {
                                mods_configuration mmc;
                                data = file.readLine().trimmed();
                                segment = data.split("--")[0];
                                if(segment.indexOf("{") > - 1)
                                {
                                    while(!file.atEnd())
                                    {
                                        data = file.readLine().trimmed();
                                        segment = data.split("--")[0];

                                        if(segment.indexOf("name") > -1)
                                        {
                                            mmc.name = segment.split("=")[1].replace(" ","").replace(",","").replace('"',"");
                                        }
                                        else if(segment.indexOf("options") > -1)
                                        {
                                            while(!file.atEnd())
                                            {
                                                QString inGroup1 = file.readLine().trimmed();
                                                QString inGroup = inGroup1.split("--")[0];
                                                if(inGroup.indexOf("description") > -1)
                                                {
                                                    mmc.option_names.push_back(inGroup.split(",")[0].split("=")[1].replace(",","").replace('"',"").replace(' "',""));
                                                    mmc.options.push_back(inGroup.split(",")[1].split("=")[1].replace(" ","").replace(",","").replace('"',"").replace('}',""));
                                                }
                                                else if(inGroup.indexOf('}') > -1)
                                                {
                                                    break;
                                                }
                                            }

                                        }
                                        else if(segment.indexOf("default") > -1)
                                        {
                                            mmc.default_settings = data.split("=")[1].replace(",","").replace('"',"").replace(" ","");
                                        }
                                        else if(segment.indexOf("label") > -1)
                                        {
                                            mmc.label = data.split("=")[1].replace(",","").replace('"',"");
                                        }
                                        else if(segment.indexOf('}') > -1)
                                        {
                                            break;
                                        }

                                    }

                                }
                                if(!mmc.name.isEmpty())
                                {
        #ifdef QT_DEBUG
                                qDebug() << mmc.name << " " << mmc.option_names[0] << " " << mmc.options[0] << " " << mmc.default_settings;
        #endif
                                    mmp.conf.push_back(mmc);
                                }
                            }
                        }

                    }
                }
#ifdef QT_DEBUG
                    qDebug() << mmp.name << " " << mmp.author << " " << mmp.description << " " << mmp.client_only_mod;
#endif
                if(!mmp.client_only_mod)
                {

                    mods.push_back(mmp);
                }
            }
        }
    }

    return true;
}

bool IOModOverridesLua::readLuaFile(std::vector<mods_properties> &mods, const QString &fileName)
{
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly);
            while(!file.atEnd())
            {
                QString msg;
                msg = file.readLine();
                msg = msg.trimmed().split("--")[0];
                if(msg.indexOf("workshop-") > -1)
                {
                    QStringList name, value;
                    QString MODID = msg.split("=")[0].split("-")[1].replace('"',"").replace("]","").replace(" ","");
                    bool enabled = msg.indexOf("true") > -1 ? true : false;
                    if(msg.indexOf("}") == - 1)
                    {
                        while(!file.atEnd())
                        {
                            QString data = file.readLine().trimmed();
                            QString segment = data.split("--")[0];
                            if(segment.indexOf("configuration_options") > -1)
                            {

                                while(!file.atEnd())
                                {
                                    data = file.readLine().trimmed();
                                    segment = data.split("--")[0];
                                    if(segment.indexOf("}") > - 1)
                                    {
                                        break;
                                    }
                                    name.push_back(segment.split("=")[0].replace(" ",""));
                                    value.push_back(segment.split("=")[1].replace(" ","").replace(',',"").replace('"',""));
                                }

                            }
                            if(segment.indexOf("}") > - 1)
                            {
                                break;
                            }
                        }
                    }

                    int i = 0;
                    for(i = 0; i < mods.size(); i++)
                    {
                        mods_properties &mm = mods[i];

                        if(!mm.id.compare(MODID))
                        {
                            if(enabled)
                            {
                                mm.isEnabled = true;
                                for(int j = 0; j < mm.conf.size(); j++)
                                {
                                    mods_configuration &cc = mm.conf[j];
                                    int pos = name.indexOf(cc.name);
                                    if(pos > -1)
                                    {
                                        QString found_value = value.at(pos);
                                        if(cc.options.indexOf(found_value) > -1)
                                        {
                                            cc.settings = found_value;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                    if(i == mods.size() + 1)
                    {
                        return false;
                    }
                }

            }
        }
    }
    return true;
}

bool IOModOverridesLua::writeLuaFile(std::vector<mods_properties> &mods, const QString &rootDirectory)
{
    return true;
}

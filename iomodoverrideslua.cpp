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
                mmp.client_only_mod = true;
                file.open(QIODevice::ReadOnly);
                while(!file.atEnd())
                {
                    QString msg;
                    msg = file.readLine();
                    msg = msg.trimmed();
                    QString msg1 = msg.split("=")[0].replace(" ","");

                    if(msg1.toLower().indexOf("name") > -1)
                    {
                        mmp.name = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("description") > -1)
                    {
                        mmp.description = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("author") > -1)
                    {
                        mmp.author = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("version") > -1)
                    {
                        mmp.version = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("forumthread") > -1)
                    {
                        mmp.forumthread = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("icon_atlas") > -1)
                    {
                        mmp.icon_atlas = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("icon") > -1)
                    {
                        mmp.icon = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("dst_compatible") > -1)
                    {
                        msg.split("=")[1].replace(" ","").replace('"',"").compare("true") ? mmp.dst_compatible = false : mmp.dst_compatible = true;
                    }
                    else if(msg1.toLower().indexOf("client_only_mod") > -1)
                    {
                        if(msg.split("=")[1].replace(" ","").replace('"',"").compare("true")) {break;}
                        else{mmp.client_only_mod = true;}
                    }
                    else if(msg1.toLower().indexOf("all_clients_require_mod") > -1)
                    {
                        msg.split("=")[1].replace(" ","").replace('"',"").compare("true") ? mmp.all_clients_require_mod = false : mmp.all_clients_require_mod = true;
                    }
                    else if(msg1.toLower().indexOf("api_version") > -1)
                    {
                        mmp.api_version = msg.split("=")[1].replace(" ","").replace('"',"");
                    }
                    else if(msg1.toLower().indexOf("configuration_options") > -1)
                    {
                        while(!file.atEnd())
                        {
                            mods_configuration mmc;
                            QString data = file.readLine().trimmed();
                            QString segment = data.split("--")[0];

                            if(segment.indexOf("name") > -1)
                            {
                                mmc.name = data.split("=")[1].replace(" ","").replace(",","").replace('"',"");
                            }
                            else if(segment.indexOf("options") > -1)
                            {
                                while(!file.atEnd())
                                {
                                    QString opt_n;
                                    QString opt;
                                    QString inGroup1 = file.readLine().trimmed();
                                    QString inGroup = inGroup1.split("--")[0];
                                    if(inGroup.indexOf("description") > -1)
                                    {
                                        opt_n = inGroup.split(",")[0].split("=")[1].replace(" ","").replace(",","").replace('"',"");
                                        opt = inGroup.split(",")[1].split("=")[1].replace(" ","").replace(",","").replace('"',"");
                                    }
                                    else if(inGroup.indexOf('}') > -1)
                                    {
                                        break;
                                    }
                                    mmc.option_names.push_back(opt_n);
                                    mmc.options.push_back(opt);
                                }

                            }
                            else if(segment.indexOf("default") > -1)
                            {
                                mmc.settings = data.split("=")[1].replace(",","").replace('"',"");
                            }

                            if(!mmc.name.isEmpty())
                            {
#ifdef QT_DEBUG
                            qDebug() << mmc.name << " " << mmc.option_names[0] << " " << mmc.options[0] << " " << mmc.settings;
#endif
                                mmp.conf.push_back(mmc);
                            }
                        }
                    }
                }
                if(!mmp.client_only_mod)
                {
#ifdef QT_DEBUG
                    qDebug() << mmp.name << " " << mmp.author << " " << mmp.description;
#endif
                    mods.push_back(mmp);
                }
            }
        }
    }

    return true;
}

bool IOModOverridesLua::readLuaFile(std::vector<QString> &mods, const QString &rootDirectory)
{
    return true;
}

bool IOModOverridesLua::writeLuaFile(std::vector<QString> &mods, const QString &rootDirectory)
{
    return true;
}

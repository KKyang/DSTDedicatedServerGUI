#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&_dst_server, SIGNAL(readyRead()), this, SLOT(writeWorldMesgOnScreen()));
    connect(&_dst_cave, SIGNAL(readyRead()), this, SLOT(writeCaveMesgOnScreen()));
    _server_token = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei/DoNotStarveTogether/") + QString(server_token);

    QFileInfo checkTokenFile(_server_token);
    if(!(checkTokenFile.exists() && checkTokenFile.isFile()))
    {
        QMessageBox::warning(this, "Warning", "Missing server token files!\nMake sure you have one somewhere else.");
        _server_token.clear();
    }
    ui->lineEdit_serverTokenLocation->setText(_server_token);

    world.resize(2);

    world[DST_WORLD].world_name = "world";
    world[DST_CAVE].world_name = "cave";
    ui->lineEdit_serverDataLocation->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei"));
    _server_found = checkServerExists(ui->lineEdit_serverDataLocation->text());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeWorldMesgOnScreen()
{
    QByteArray result = _dst_server.readAllStandardOutput();
#ifdef _DEBUG
        qDebug() << result.data();
#endif
    ui->textBrowser_world->append(QString(result));
}

void MainWindow::writeCaveMesgOnScreen()
{
    QByteArray result = _dst_cave.readAllStandardOutput();
#ifdef _DEBUG
        qDebug() << result.data();
#endif
    ui->textBrowser_cave->append(QString(result));
}

bool MainWindow::checkServerExists(QString dstds_path, bool reload_template)
{
    QDir checkFolder;
    if(checkFolder.exists(dstds_path+ QString("/") + QString(folder_world)) && checkFolder.exists(dstds_path+ QString("/") + QString(folder_cave)))
    {
        if(!readINI(DST_CAVE, dstds_path + QString("/") + QString(folder_world) + QString("/") + QString(settings_ini)))
        {
            QMessageBox::critical(this, "Error", "Cannot found ini files in cave folder!");
            return false;
        }
        if(!readINI(DST_WORLD, dstds_path + QString("/") + QString(folder_cave) + QString("/") + QString(settings_ini), true))
        {
            QMessageBox::critical(this, "Error", "Cannot found ini files in world folder!");
            return false;
        }

        disableSetttingsWhenServerExists(true);
        return true;
    }
    else if(reload_template)
    {
        readINI(DST_CAVE, QString("template/") + QString(ini_cave));
        readINI(DST_WORLD, QString("template/") + QString(ini_world));
        disableSetttingsWhenServerExists(false);
    }
    return false;
}

bool MainWindow::firstServerSetup()
{
    _dstds_folder = ui->lineEdit_serverDataLocation->text();

    //Check if the server data folder exists.
    QDir checkFolder;
    if(!checkFolder.exists(_dstds_folder))
    {
        if(QMessageBox::warning(this, "Klei folder not found.", "Folder not found, shall I create one?", QMessageBox::Yes|QMessageBox::No)
                == QMessageBox::Yes)
        {
            checkFolder.mkpath(_dstds_folder);
        }
        else
        {
            return false;
        }
    }

    //Check if the subfolder exists. Both folder named in dst_server, dst_

    _dstds_folder = ui->lineEdit_serverDataLocation->text() + QString("/") + QString(folder_world);
    if(!checkFolder.exists(_dstds_folder))
    {
        if(QMessageBox::warning(this, "GUI server folder not found.", "Folder not found, shall I create one?", QMessageBox::Yes|QMessageBox::No)
                == QMessageBox::No)
        {
            return false;
        }
        checkFolder.mkpath(_dstds_folder);
        writeLua(DST_WORLD, _dstds_folder + QString("/") + QString(override_lua));
        changeSettings(DST_WORLD, "default_server_name", ui->lineEdit_servername->text());
        changeSettings(DST_WORLD, "default_server_description", ui->lineEdit_serverDescription->text());
        changeSettings(DST_WORLD, "max_players", ui->spinBox_serverMaxPlayers->text());
        ui->radioButton_pvpYes->isChecked() ? changeSettings(DST_WORLD, "pvp", "true") :
                                              changeSettings(DST_WORLD, "pvp", "false");
        changeSettings(DST_WORLD, "server_password", ui->lineEdit_serverPassword->text());
        changeSettings(DST_WORLD, "server_save_slot", ui->comboBox_serverSaveSlot->currentText());
        changeSettings(DST_WORLD, "server_intention", ui->comboBox_serverIntention->currentText());
        changeSettings(DST_WORLD, "game_mode", ui->comboBox_gamemode->currentText());

        writeINI(DST_WORLD,_dstds_folder + QString("/") + QString(settings_ini));

    }

    _dstds_folder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei/") + QString(folder_cave);
    if(!checkFolder.exists(_dstds_folder))
    {
        if(QMessageBox::warning(this, "GUI cave folder not found.", "Folder not found, shall I create one?", QMessageBox::Yes|QMessageBox::No)
                == QMessageBox::No)
        {
            return false;
        }

        checkFolder.mkpath(_dstds_folder);
        writeLua(DST_CAVE, _dstds_folder + QString("/") + QString(override_lua));
        changeSettings(DST_CAVE, "default_server_name", ui->lineEdit_servername->text());
        changeSettings(DST_CAVE, "default_server_description", ui->lineEdit_serverDescription->text());
        changeSettings(DST_CAVE, "max_players", ui->spinBox_serverMaxPlayers->text());
        ui->radioButton_pvpYes->isChecked() ? changeSettings(DST_CAVE, "pvp", "true") :
                                              changeSettings(DST_CAVE, "pvp", "false");
        //changeSettings(DST_CAVE, "server_password", ui->lineEdit_serverPassword->text());
        changeSettings(DST_CAVE, "server_save_slot", ui->comboBox_serverSaveSlot->currentText());
        changeSettings(DST_CAVE, "server_intention", ui->comboBox_serverIntention->currentText());
        changeSettings(DST_CAVE, "game_mode", ui->comboBox_gamemode->currentText());
        writeINI(DST_CAVE,_dstds_folder + QString("/") + QString(settings_ini));
    }
}

void MainWindow::changeSettings(int world_num, QString name, QString value)
{
    for(int i = 0; i < world[world_num].ini.size(); i++)
    {
        properties &p = world[world_num].ini[i];
        if(!p.name.compare(name))
        {
            p.settings = value;
#ifdef QT_DEBUG
            qDebug() << world[world_num].ini[i].name << " " << world[world_num].ini[i].settings;
#endif
            break;
        }
    }
}

void MainWindow::changeBasicSettingsUI(QString name, QString value)
{
    if(!name.compare("default_server_name")){ui->lineEdit_servername->setText(value);}
    else if(!name.compare("default_server_description")){ui->lineEdit_serverDescription->setText(value);}
    else if(!name.compare("max_players")){ui->spinBox_serverMaxPlayers->setValue(value.toInt());}
    else if(!name.compare("server_password")){ui->lineEdit_serverPassword->setText(value);}
    else if(!name.compare("server_save_slot")){ui->comboBox_serverSaveSlot->setCurrentIndex(value.toInt() - 1);}
    else if(!name.compare("server_intention"))
    {
        if(!name.compare("cooperative"))
            ui->comboBox_serverIntention->setCurrentIndex(0);
    }
    else if(!name.compare("game_mode"))
    {
        if(!name.compare("Endless"))
            ui->comboBox_gamemode->setCurrentIndex(0);
        else if(!name.compare("Survival"))
            ui->comboBox_gamemode->setCurrentIndex(1);
        else if(!name.compare("Wilderness"))
            ui->comboBox_gamemode->setCurrentIndex(2);
    }
    else if(!name.compare("pvp"))
    {
        (!value.compare("true")) ? ui->radioButton_pvpYes->setChecked(true) : ui->radioButton_pvpNo->setChecked(true);
    }
}

bool MainWindow::readINI(int world_num, QString file_path, bool ui_c)
{
    QSettings settings(file_path, QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    for(int i = 0; i < groups.size(); i++)
    {
        settings.beginGroup(groups[i]);
        QStringList keys = settings.allKeys();
        for(int j = 0; j < keys.size(); j++)
        {
            properties ini;
            ini.group = groups[i];
            ini.name = keys[j];
            ini.settings = settings.value(keys[j]).toString();

            world[world_num].ini.push_back(ini);
            changeBasicSettingsUI(ini.name, ini.settings);
        }
        settings.endGroup();
    }
    return true;
}

bool MainWindow::writeINI(int world_num, QString file_path)
{
    QSettings settings(file_path, QSettings::IniFormat);
    for(int i = 0; i < world[world_num].ini.size(); i++)
    {
        properties &ini = world[world_num].ini[i];
        settings.setValue(ini.group + QString("/") + ini.name, ini.settings);
    }
    return true;
}

// Custom lua content in app currently not supported. Please modify the template file directly to work.
bool MainWindow::readLua(int world_num, QString file_path)
{
    if(world_num == DST_WORLD)
    {
        //        QFile file_r("template/worldgenoverride_cave.lua");
        //        if(!file_r.open(QIODevice::ReadOnly))
        //        {
        //            return false;
        //        }

        //        if(file.open(QIODevice::ReadWrite))
        //        {
        //            QTextStream stream(&file);

        //        }
    }
    else if(world_num == DST_CAVE)
    {

    }
    return true;
}

// Custom lua content in app currently not supported. Please modify the template file directly to work.
bool MainWindow::writeLua(int world_num, QString file_path)
{
    //QFile file_w(file_path);
    if(world_num == DST_WORLD)
    {
        if(world[0].pro.empty())
        {
            return QFile::copy("template/worldgenoverride_world.lua", file_path);
        }
        else
        {

        }
    }
    else if(world_num == DST_CAVE)
    {
        return QFile::copy("template/worldgenoverride_cave.lua", file_path);
    }
}

void MainWindow::disableWidgetsWhenStartServer(bool status)
{

}

void MainWindow::disableSetttingsWhenServerExists(bool status)
{
    ui->comboBox_gamemode->setEnabled(!status);
    ui->comboBox_serverSaveSlot->setEnabled(!status);
}

void MainWindow::on_pushButton_startServer_clicked()
{
    //Check token file exists.
    QFileInfo checkfile(ui->lineEdit_serverTokenLocation->text());
    if(!(checkfile.exists() && checkfile.isFile()))
    {
        QMessageBox::critical(this, "Error", "No token found!");
        return;
    }

    //Check if dst exe exists.
    _dstds_exe = ui->lineEdit_dedicatedServerLocation->text() + "/bin/dontstarve_dedicated_server_nullrenderer.exe";
    checkfile.setFile(_dstds_exe);
    if(!(checkfile.exists() && checkfile.isFile()))
    {
        QMessageBox::critical(this, "Error", "Don't Starve Together Dedicated Server not found!");
        return;
    }

    _dstds_folder = ui->lineEdit_serverDataLocation->text();

    //Check again if the folder changed.
    _server_found = checkServerExists(_dstds_folder, false);

    if(!_server_found)
        firstServerSetup();



    /*
     * cd C:\Program Files (x86)\Steam\SteamApps\common\Don't Starve Together Dedicated Server\bin
     * dontstarve_dedicated_server_nullrenderer -conf_dir DST_Server -console
     *
     * cd C:\Program Files (x86)\Steam\SteamApps\common\Don't Starve Together Dedicated Server\bin
     * dontstarve_dedicated_server_nullrenderer -conf_dir DST_Cave -console
     * */
      //QString startup_cmd;
      //    _dst_server.start("cmd");
      //    _dst_cave.start("cmd");
//    if(_dstds_exe[0].toUpper() != QChar("C"))
//    {
//        startup_cmd = _dstds_exe[0].toUpper() + QString(": ");
//    }
//    startup_cmd = startup_cmd + QString("D: cd ") + _dstds_exe + QString(" -conf_dir ") + QString(folder_world) + QString(" -console\n");
//    _dst_server.write(startup_cmd);
//    startup_cmd = startup_cmd + QString("D: cd ") + _dstds_exe + QString(" -conf_dir ") + QString(folder_cave) + QString(" -console\n");
//    _dst_cave.write(startup_cmd);
    QStringList startup_cmd;
    startup_cmd << "-conf_dir" << QString(folder_world) << QString(" -console");
    _dst_server.start(_dstds_exe, startup_cmd);
    startup_cmd << "-conf_dir" << QString(folder_cave) << QString(" -console");
    _dst_cave.start(_dstds_exe, startup_cmd);
}

void MainWindow::on_pushButton_stopServer_clicked()
{
    if(_dst_server.state() == QProcess::Running)
    {
        _dst_server.write("c_shutdown()");
        _dst_server.write("\n");
    }
    if(_dst_cave.state() == QProcess::Running)
    {
        _dst_cave.write("c_shutdown()");
        _dst_cave.write("\n");
    }
}

void MainWindow::on_pushButton_restartServer_clicked()
{

}

void MainWindow::on_lineEdit_worldEdit_returnPressed()
{
    if(_dst_server.state() == QProcess::Running)
    {
        _dst_server.write(ui->lineEdit_worldEdit->text().toStdString().c_str());
        _dst_server.write("\n");
        ui->lineEdit_worldEdit->clear();
    }
}

void MainWindow::on_lineEdit_caveEdit_returnPressed()
{
    if(_dst_server.state() == QProcess::Running)
    {
        _dst_cave.write(ui->lineEdit_caveEdit->text().toStdString().c_str());
        _dst_cave.write("\n");
        ui->lineEdit_caveEdit->clear();
    }
}

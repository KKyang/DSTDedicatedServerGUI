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

    readGUIINISettings();

    connect(&_dst_server, SIGNAL(readyRead()), this, SLOT(writeWorldMesgOnScreen()));
    connect(&_dst_cave, SIGNAL(readyRead()), this, SLOT(writeCaveMesgOnScreen()));
    connect(&_update_mod, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(modeUpdated(int,QProcess::ExitStatus)));
    world.resize(2);
    world[DST_WORLD].world_name = "world";
    world[DST_CAVE].world_name = "cave";
    ui->lineEdit_serverDataLocation->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei"));
    _server_found = checkServerExists(ui->lineEdit_serverDataLocation->text());

    group = new QButtonGroup(parent);
    group->addButton(ui->pushButton_startServer);
    group->addButton(ui->pushButton_stopServer);
    group->setExclusive(true);
}

MainWindow::~MainWindow()
{
    if(_dst_server.state() == QProcess::Running)
    {
        _dst_server.write("c_shutdown()");
        _dst_server.write("\n");
        _dst_server.waitForFinished(10000);
    }
    _dst_server.close();

    if(_dst_cave.state() == QProcess::Running)
    {
        _dst_cave.write("c_shutdown()");
        _dst_cave.write("\n");
        _dst_cave.waitForFinished(10000);
    }
    _dst_cave.close();
    writeGUIINISettings();

    delete ui;
}

//GUI settings start
void MainWindow::readGUIINISettings()
{
    QFileInfo checkfile("guisettings.ini");
    if((checkfile.exists() && checkfile.isFile()))
    {
        QSettings settings("guisettings.ini", QSettings::IniFormat);
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

                if(!ini.name.compare("DST Server Root Directory")){ui->lineEdit_dedicatedServerLocation->setText(ini.settings);}
                else if(!ini.name.compare("DST token location")){_server_token = ini.settings;}
            }
            settings.endGroup();
        }
    }

    if(ui->lineEdit_serverTokenLocation->text().isEmpty())
    {
        _server_token = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei/DoNotStarveTogether/") + QString(server_token);

        QFileInfo checkTokenFile(_server_token);
        if(!(checkTokenFile.exists() && checkTokenFile.isFile()))
        {
            QMessageBox::warning(this, "Warning", "Missing server token files!\nMake sure you have one somewhere else.");
            _server_token.clear();
        }
    }

    ui->lineEdit_serverTokenLocation->setText(_server_token);
}

void MainWindow::writeGUIINISettings()
{
    _gui_ini.push_back(properties("Basic","DST Server Root Directory", ui->lineEdit_dedicatedServerLocation->text()));
    _gui_ini.push_back(properties("Basic", "DST token location", ui->lineEdit_serverTokenLocation->text()));

    QSettings settings("guisettings.ini", QSettings::IniFormat);
    for(int i = 0; i < _gui_ini.size(); i++)
    {
        properties &ini = _gui_ini[i];
        settings.setValue(ini.group + QString("/") + ini.name, ini.settings);
    }
}
//GUI settings end

//GUI signal and slots start
void MainWindow::writeWorldMesgOnScreen()
{
    QByteArray result = _dst_server.readAllStandardOutput();
#ifdef _DEBUG
        qDebug() << result.data();
#endif
    ui->textBrowser_world->insertPlainText(QString(result));
    ui->textBrowser_world->verticalScrollBar()->setValue(ui->textBrowser_world->verticalScrollBar()->maximum());
}

void MainWindow::writeCaveMesgOnScreen()
{
    QByteArray result = _dst_cave.readAllStandardOutput();
#ifdef _DEBUG
        qDebug() << result.data();
#endif
    ui->textBrowser_cave->insertPlainText(QString(result));
    ui->textBrowser_cave->verticalScrollBar()->setValue(ui->textBrowser_cave->verticalScrollBar()->maximum());
}

void MainWindow::modeUpdated(int code,QProcess::ExitStatus status)
{
    ui->statusBar->showMessage("Update finished. Starting dedicated servers.");
    //Start both servers
    QStringList startup_cmd;
    startup_cmd << "-conf_dir" << QString(folder_world) << QString(" -console") << QString("-skip_update_server_mods");
    _dst_server.setWorkingDirectory(ui->lineEdit_dedicatedServerLocation->text() + QString("/bin"));
    _dst_server.start(_dstds_exe, startup_cmd);

    startup_cmd.clear();
    startup_cmd << "-conf_dir" << QString(folder_cave) << QString("-console") << QString("-skip_update_server_mods");
    _dst_cave.setWorkingDirectory(ui->lineEdit_dedicatedServerLocation->text() + QString("/bin"));
    _dst_cave.start(_dstds_exe, startup_cmd);
}
//GUI signal and slots end

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
        if(QMessageBox::warning(this, "GUI server folder not found.", "World folder not found, shall I create one?", QMessageBox::Yes|QMessageBox::No)
                == QMessageBox::No)
        {
            return false;
        }
        checkFolder.mkpath(_dstds_folder);
        QFile::copy(ui->lineEdit_serverTokenLocation->text(), QString(_dstds_folder + QString("/") + QString(server_token)));
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
        if(QMessageBox::warning(this, "GUI cave folder not found.", "Cave folder not found, shall I create one?", QMessageBox::Yes|QMessageBox::No)
                == QMessageBox::No)
        {
            return false;
        }

        checkFolder.mkpath(_dstds_folder);
        QFile::copy(ui->lineEdit_serverTokenLocation->text(), _dstds_folder + QString("/") + QString(server_token));
        writeLua(DST_CAVE, _dstds_folder + QString("/") + QString(override_lua));
        changeSettings(DST_CAVE, "default_server_name", ui->lineEdit_servername->text());
        changeSettings(DST_CAVE, "default_server_description", ui->lineEdit_serverDescription->text());
        changeSettings(DST_CAVE, "max_players", ui->spinBox_serverMaxPlayers->text());
        ui->radioButton_pvpYes->isChecked() ? changeSettings(DST_CAVE, "pvp", "true") :
                                              changeSettings(DST_CAVE, "pvp", "false");
        changeSettings(DST_CAVE, "server_password", ui->lineEdit_serverPassword->text());
        changeSettings(DST_CAVE, "server_save_slot", ui->comboBox_serverSaveSlot->currentText());
        changeSettings(DST_CAVE, "server_intention", ui->comboBox_serverIntention->currentText());
        changeSettings(DST_CAVE, "game_mode", ui->comboBox_gamemode->currentText().toLower());
        writeINI(DST_CAVE,_dstds_folder + QString("/") + QString(settings_ini));
    }
    _server_found = true;
    return true;
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
    ui->lineEdit_dedicatedServerLocation->setEnabled(!status);
    ui->lineEdit_serverTokenLocation->setEnabled(!status);
    ui->lineEdit_servername->setEnabled(!status);
    ui->lineEdit_serverDataLocation->setEnabled(!status);
    ui->lineEdit_serverDescription->setEnabled(!status);
    ui->spinBox_serverMaxPlayers->setEnabled(!status);
    ui->radioButton_pvpYes->setEnabled(!status);
    ui->radioButton_pvpNo->setEnabled(!status);
    ui->lineEdit_serverPassword->setEnabled(!status);
    ui->comboBox_serverSaveSlot->setEnabled(!status);
    ui->comboBox_serverIntention->setEnabled(!status);
    ui->comboBox_gamemode->setEnabled(!status);

    if(_server_found)
    {
        disableSetttingsWhenServerExists(true);
    }
}

void MainWindow::disableSetttingsWhenServerExists(bool status)
{
    ui->comboBox_gamemode->setEnabled(!status);
    ui->comboBox_serverSaveSlot->setEnabled(!status);
}

void MainWindow::on_pushButton_startServer_clicked()
{
    disableWidgetsWhenStartServer(true);
    //Check token file exists.
    ui->statusBar->showMessage("Check if token file exists.");
    QFileInfo checkfile(ui->lineEdit_serverTokenLocation->text());
    if(!(checkfile.exists() && checkfile.isFile()) && !_server_found)
    {
        QMessageBox::critical(this, "Error", "No token found!");
        ui->statusBar->showMessage("No server token. Aborting.");
        disableWidgetsWhenStartServer(false);
        return;
    }

    //Check if dst exe exists.
    ui->statusBar->showMessage("Check if don't starve together dedicated server exists.");
    _dstds_exe = ui->lineEdit_dedicatedServerLocation->text() + "/bin/dontstarve_dedicated_server_nullrenderer.exe";
    checkfile.setFile(_dstds_exe);
    if(!(checkfile.exists() && checkfile.isFile()))
    {
        QMessageBox::critical(this, "Error", "Don't Starve Together Dedicated Server not found!");
        ui->statusBar->showMessage("No dedicated server executable detected. Aborting.");
        disableWidgetsWhenStartServer(false);
        return;
    }



    //Check again if the folder changed.
    ui->statusBar->showMessage("Check if there is an existing server.");
    _server_found = checkServerExists(_dstds_folder, false);

    if(!_server_found)
    {
        ui->statusBar->showMessage("Creating new server.");
        if(!firstServerSetup())
        {
            ui->statusBar->showMessage("Something went wrong shen creating a new server.");
            disableWidgetsWhenStartServer(false);
        }
    }

    /*
     * cd C:\Program Files (x86)\Steam\SteamApps\common\Don't Starve Together Dedicated Server\bin
     * dontstarve_dedicated_server_nullrenderer -conf_dir DST_Server -console
     *
     * cd C:\Program Files (x86)\Steam\SteamApps\common\Don't Starve Together Dedicated Server\bin
     * dontstarve_dedicated_server_nullrenderer -conf_dir DST_Cave -console
     * */

    QStringList startup_cmd;
    //Update server mod
    startup_cmd << "-only_update_server_mods";
    ui->statusBar->showMessage("Updating server mods.");
    _update_mod.setWorkingDirectory(ui->lineEdit_dedicatedServerLocation->text() + QString("/bin"));
    _update_mod.start(_dstds_exe, startup_cmd);
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
    disableWidgetsWhenStartServer(false);
}

void MainWindow::on_pushButton_restartServer_clicked()
{

}

void MainWindow::on_lineEdit_worldEdit_returnPressed()
{
    if(!ui->lineEdit_worldEdit->text().compare("c_shutdown()"))
    {
        ui->statusBar->showMessage("Please use stop server button or use restart command instead.");
        return;
    }
    if(_dst_server.state() == QProcess::Running)
    {
        _dst_server.write(ui->lineEdit_worldEdit->text().toStdString().c_str());
        _dst_server.write("\n");
        ui->lineEdit_worldEdit->clear();
    }
    else if(!ui->lineEdit_worldEdit->text().compare("gui_startServer"))
    {
        //Still working on it.
    }
}

void MainWindow::on_lineEdit_caveEdit_returnPressed()
{
    if(!ui->lineEdit_caveEdit->text().compare("c_shutdown()"))
    {
        ui->statusBar->showMessage("Please use stop server button or use restart command instead.");
        return;
    }
    if(_dst_server.state() == QProcess::Running)
    {
        _dst_cave.write(ui->lineEdit_caveEdit->text().toStdString().c_str());
        _dst_cave.write("\n");
        ui->lineEdit_caveEdit->clear();
    }
    else if(!ui->lineEdit_caveEdit->text().compare("gui_startServer"))
    {
        //Still working on it.
    }
}

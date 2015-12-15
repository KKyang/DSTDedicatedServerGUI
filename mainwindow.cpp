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
    _server_token = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei/DoNotStarveTogether/server_token.txt");

    QFileInfo checkTokenFile(_server_token);
    if(!(checkTokenFile.exists() && checkTokenFile.isFile()))
    {
        QMessageBox::warning(this, "Warning", "Missing server token files!\nMake sure you have one somewhere else.");
        _server_token.clear();
    }
    world.resize(2);

    world[DST_WORLD].world_name = "world";
    world[DST_CAVE].world_name = "cave";
    ui->lineEdit_serverDataLocation->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/Klei"));
    checkServerExists(ui->lineEdit_serverDataLocation->text());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkServerExists(QString dstds_path)
{
    QDir checkFolder;
    if(checkFolder.exists(dstds_path+ QString("/") + QString(folder_world)) && checkFolder.exists(dstds_path+ QString("/") + QString(folder_cave)))
    {

        readINI(DST_CAVE, dstds_path + QString("/") + QString(folder_world) + QString("/") + QString("settings.ini"));
        readINI(DST_WORLD, dstds_path + QString("/") + QString(folder_cave) + QString("/") + QString("settings.ini"), true);
        disableSetttingsWhenServerExists(true);
    }
    else
    {
        readINI(DST_CAVE, "template/settings_cave.ini");
        readINI(DST_WORLD, "template/settings_world.ini");
        disableSetttingsWhenServerExists(false);
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
            qDebug() << world[world_num].ini[i].name << " " << world[world_num].ini[i].settings;
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
    _dstds_exe = ui->lineEdit_dedicatedServerLocation->text() + "/dontstarve_dedicated_server_nullrenderer.exe";
    QFileInfo checkfile(_dstds_exe);

    //Check if dst exe exists.
    if(!(checkfile.exists() && checkfile.isFile()))
    {
        QMessageBox::critical(this, "Error", "Don't Starve Together Dedicated Server not found!");
        return;
    }
    _dstds_folder = ui->lineEdit_serverDataLocation->text();

    //Check if the server data folder exists.
    QDir checkFolder;
    if(!checkFolder.exists(_dstds_folder))
    {
        if(QMessageBox::warning(this, "Folder not found.", "Folder not found, shall I create one?", QMessageBox::Yes|QMessageBox::No)
                == QMessageBox::Yes)
        {
            checkFolder.mkpath(_dstds_folder);
        }
        else
        {
            return;
        }
    }

    //Check if the subfolder exists. Both folder named in dst_server, dst_

    _dstds_folder = ui->lineEdit_serverDataLocation->text() + QString("/") + QString(folder_world);
    if(!checkFolder.exists(_dstds_folder))
    {
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

void MainWindow::on_pushButton_stopServer_clicked()
{

}

void MainWindow::on_pushButton_restartServer_clicked()
{

}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define server_token "server_token.txt"
#define override_lua "worldgenoverride.lua"
#define mod_override_lua "modoverrides.lua"

#define settings_ini "settings.ini"
#define folder_world "gui_dst_server"
#define folder_cave  "gui_dst_cave"

#define override_world_lua_template "worldgenoverride_world.lua"
#define override_cave_lua_template "worldgenoverride_cave.lua"
#define ini_world "settings_world.ini"
#define ini_cave  "settings_cave.ini"

#include <QMainWindow>
#include <QButtonGroup>
#include <QComboBox>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QScrollBar>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>

#include "dstdatastructure.h"
#include "ioworldgenoverridelua.h"
#include "iomodoverrideslua.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void writeWorldMesgOnScreen();
    void writeCaveMesgOnScreen();
    void modeUpdated(int code,QProcess::ExitStatus status);

    void on_pushButton_startServer_clicked();

    void on_pushButton_stopServer_clicked();

    void on_pushButton_restartServer_clicked();

    void on_lineEdit_worldEdit_returnPressed();

    void on_lineEdit_caveEdit_returnPressed();

    void on_lineEdit_servername_editingFinished();

    void on_lineEdit_serverDescription_editingFinished();

    void on_lineEdit_serverPassword_editingFinished();

    void on_spinBox_serverMaxPlayers_editingFinished();

    void on_radioButton_pvpYes_toggled(bool checked);

    void on_comboBox_gamemode_currentIndexChanged(const QString &arg1);

    void on_comboBox_serverSaveSlot_currentIndexChanged(const QString &arg1);

    void on_comboBox_serverIntention_currentIndexChanged(const QString &arg1);

    void on_pushButton_rescanMods_clicked();

    void on_lineEdit_dedicatedServerLocation_editingFinished();

private:
    Ui::MainWindow *ui;
    //Main app ini settings
    void readGUIINISettings();
    void writeGUIINISettings();

    std::vector<properties> _gui_ini;

    //DST related
    bool checkServerExists(QString dstds_path, bool reload_template = true);
    bool firstServerSetup();

    void changeINISettings(int world_num, QString name);
    bool readINI(int world_num, QString file_path, bool ui_c = false);
    bool writeINI(int world_num, QString file_path);
    void writeINIToGUI(int world_num);
    bool readLua(int world_num, QString file_path);
    bool writeLua(int world_num, QString file_path);
    void writeLuaToGUI(int world_num);

    bool readInstalledModLua();
    bool readModLua(int world_num, QString file_path);
    bool writeModLua(int world_num, QString file_path);
    void writeModLuaToGUI(int world_num);

    QProcess _update_mod;
    QProcess _dst_server;
    QProcess _dst_cave;

    bool _server_found = false;

    QString _dstds_exe;
    QString _dstds_folder;
    QString _server_token;

    std::vector<DSTSettings> world;
    std::vector<mods_properties> world_mods;

    //UI
    void disableWidgetsWhenStartServer(bool status);
    void disableSetttingsWhenServerExists(bool status);
    QButtonGroup *group;
};

#endif // MAINWINDOW_H

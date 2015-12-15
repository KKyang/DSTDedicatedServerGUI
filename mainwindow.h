#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define server_token "server_token.txt"
#define override_lua "worldgenoverride.lua"
#define settings_ini "settings.ini"
#define folder_world "gui_dst_server"
#define folder_cave  "gui_dst_cave"

#define override_world_lua_template "worldgenoverride_world.lua"
#define override_cave_lua_template "worldgenoverride_cave.lua"
#define ini_world "settings_world.ini"
#define ini_cave  "settings_cave.ini"

#include <QMainWindow>
#include <QButtonGroup>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QScrollBar>
#include <QSettings>
#include <QStandardPaths>


struct properties{
    properties(){}
    properties(QString g, QString n, QString s) : group(g), name(n), settings(s){}
    QString group;
    QString name;
    QString settings;
};

struct DSTSettings{
    QString world_name;
    std::vector<properties> ini;
    std::vector<properties> pro;
};

enum{
    DST_WORLD = 0,
    DST_CAVE = 1
};

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

private:
    Ui::MainWindow *ui;
    //Main app ini settings
    void readGUIINISettings();
    void writeGUIINISettings();

    std::vector<properties> _gui_ini;

    //DST related
    bool checkServerExists(QString dstds_path, bool reload_template = true);
    bool firstServerSetup();

    void changeSettings(int world_num, QString name, QString value);
    void changeBasicSettingsUI(QString name, QString value);
    bool readINI(int world_num, QString file_path, bool ui_c = false);
    bool writeINI(int world_num, QString file_path);
    bool readLua(int world_num, QString file_path);
    bool writeLua(int world_num, QString file_path);

    QProcess _update_mod;
    QProcess _dst_server;
    QProcess _dst_cave;

    bool _server_found = false;

    QString _dstds_exe;
    QString _dstds_folder;
    QString _server_token;

    std::vector<DSTSettings> world;

    //UI
    void disableWidgetsWhenStartServer(bool status);
    void disableSetttingsWhenServerExists(bool status);
    QButtonGroup *group;
};

#endif // MAINWINDOW_H

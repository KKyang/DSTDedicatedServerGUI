#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define override_lua "worldgenoverride.lua"
#define settings_ini "settings.ini"
#define folder_world "gui_dst_server"
#define folder_cave  "gui_dst_cave"

#define override_world_lua_template "worldgenoverride_world.lua"
#define override_cave_lua_template "worldgenoverride_cave.lua"
#define ini_world "settings_world.ini"
#define ini_cave  "settings_cave.ini"

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>


struct properties{
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
    void on_pushButton_startServer_clicked();

    void on_pushButton_stopServer_clicked();

    void on_pushButton_restartServer_clicked();

private:
    void checkServerExists(QString dstds_path);

    void changeSettings(int world_num, QString name, QString value);
    void changeBasicSettingsUI(QString name, QString value);
    bool readINI(int world_num, QString file_path, bool ui_c = false);
    bool writeINI(int world_num, QString file_path);
    bool readLua(int world_num, QString file_path);
    bool writeLua(int world_num, QString file_path);

    void disableWidgetsWhenStartServer(bool status);
    void disableSetttingsWhenServerExists(bool status);

    Ui::MainWindow *ui;
    QProcess _dst_server;
    QProcess _dst_cave;

    QString _dstds_exe;
    QString _dstds_folder;
    QString _server_token;

    std::vector<DSTSettings> world;
};

#endif // MAINWINDOW_H

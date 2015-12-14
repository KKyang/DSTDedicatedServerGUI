#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

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
    Ui::MainWindow *ui;
    QProcess _dst_server;
    QProcess _dst_cave;

    QString _dstds_exe;
    QString _dstds_folder;
    QString _server_token;
};

#endif // MAINWINDOW_H

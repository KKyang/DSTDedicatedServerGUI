#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _server_token = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("\Klei\DoNotStarveTogether\server_token.txt");

    QFileInfo checkTokenFile(_server_token);
    if(!(checkTokenFile.exists() && checkTokenFile.isFile()))
    {
        QMessageBox::warning(this, "Warning", "Missing server token files!\nMake sure you have one somewhere else.");
        _server_token.clear();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_startServer_clicked()
{
    _dstds_exe = ui->lineEdit_dedicatedServerLocation->text() + "\dontstarve_dedicated_server_nullrenderer.exe";
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
    _dstds_folder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("\Klei\dst_server");
    if(!checkFolder.exists(_dstds_folder))
    {
        checkFolder.mkpath(_dstds_folder);

    }
    else
    {

    }
    _dstds_folder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("\Klei\dst_cave");
    if(!checkFolder.exists(_dstds_folder))
    {
        checkFolder.mkpath(_dstds_folder);

    }
    else
    {

    }

}

void MainWindow::on_pushButton_stopServer_clicked()
{

}

void MainWindow::on_pushButton_restartServer_clicked()
{

}

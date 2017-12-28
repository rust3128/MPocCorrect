#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "deletesaleorderdialog.h"
#include "lostchekerdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConSetup_triggered()
{
    ConnectDialog *connDlg = new ConnectDialog();
    connDlg->exec();
}

void MainWindow::on_actionZerro_triggered()
{
    DeleteSaleorderDialog *delSaleDlg = new DeleteSaleorderDialog();
    this->setCentralWidget(delSaleDlg);
}

void MainWindow::on_actionLostCheck_triggered()
{
    LostChekerDialog *lostCheck = new LostChekerDialog();
    this->setCentralWidget(lostCheck);
}

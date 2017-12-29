#include "lostchekerdialog.h"
#include "ui_lostchekerdialog.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>

LostChekerDialog::LostChekerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LostChekerDialog)
{
    ui->setupUi(this);

    createModelTerminals();
    ui->labelAZS->hide();
    ui->pushButtonConnect->hide();
}

LostChekerDialog::~LostChekerDialog()
{
    delete ui;
}

void LostChekerDialog::createModelTerminals()
{
    QSqlDatabase dbc = QSqlDatabase::database("central");
    modelTerminals = new QSqlQueryModel();
    QString strSQL = "SELECT t.TERMINAL_ID, TRIM(t.NAME) AS AZS, c.SERVER_NAME, c.DB_NAME, c.CON_LOGIN, c.CON_PASSWORD from TERMINALS t "
            "LEFT JOIN CONNECTIONS c ON t.TERMINAL_ID=c.TERMINAL_ID "
            "where t.TERMINALTYPE=3 and t.ISACTIVE='T' and c.CONNECT_ID=1 "
            "order by t.TERMINAL_ID";
    modelTerminals->setQuery(strSQL,dbc);


}

void LostChekerDialog::on_lineEditTerminal_textChanged(const QString &arg1)
{
    ui->labelAZS->hide();
    ui->pushButtonConnect->hide();
    for(int i=0;i<modelTerminals->rowCount();++i){
        if(arg1.toInt() == modelTerminals->data(modelTerminals->index(i,0)).toInt()){
            ui->labelAZS->setText(modelTerminals->data(modelTerminals->index(i,1)).toString());
            ui->labelAZS->show();
            curTerminal = modelTerminals->record(i);
            ui->pushButtonConnect->show();
            break;
        }

    }
}

void LostChekerDialog::on_pushButtonConnect_clicked()
{
//    qDebug() << curTerminal.value("SERVER_NAME").toString();
    QThread *thread = new QThread;
    dbAzs = new AzsConnect(curTerminal);
    dbAzs->moveToThread(thread);
    connect(thread,SIGNAL(started()),dbAzs,SLOT(getAzsInfo()));
    connect(thread,SIGNAL(started()),this,SLOT(startGetInfo()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishGetInfo()));

    thread->start();


}

void LostChekerDialog::startGetInfo()
{
    qDebug() << "Start" << QDateTime::currentDateTime().toString();
}

void LostChekerDialog::finishGetInfo()
{
    qDebug() << "Finis" << QDateTime::currentDateTime().toString();
}

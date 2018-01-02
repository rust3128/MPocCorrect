#include "lostchekerdialog.h"
#include "ui_lostchekerdialog.h"

#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QMessageBox>

LostChekerDialog::LostChekerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LostChekerDialog)
{
    ui->setupUi(this);

    createModelTerminals();
    ui->labelAZS->hide();
    ui->pushButtonConnect->hide();
    ui->frameProgress->hide();
    ui->groupBoxDataChek->hide();
    ui->labelProgress->setText("Получение конфигурации АЗС..");
    modelShifts = new QSqlQueryModel();

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
    pi = new QProgressIndicator();
    QThread *thread = new QThread;
    dbAzs = new AzsConnect(curTerminal);
    qDebug() << "Push button" << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss.zzz");
    connect(thread,SIGNAL(started()),this,SLOT(startGetInfo()));
    connect(thread,SIGNAL(started()),dbAzs,SLOT(getAzsInfo()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishGetInfo()));
    connect(dbAzs,SIGNAL(fin()),thread,SLOT(terminate()));
    connect(dbAzs, SIGNAL(connectionError(QString)),this,SLOT(errogGetInfo(QString)));


    thread->start();
    dbAzs->moveToThread(thread);

}

void LostChekerDialog::startGetInfo()
{

    qDebug() << "Start" << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss.zzz");
    ui->frameProgress->show();
    pi->setColor(Qt::darkGreen);
    pi->startAnimation();

//    QFrame* frame = new QFrame;
//    QHBoxLayout* hbl = new QHBoxLayout(frame);
//    QVBoxLayout* vbl = new QVBoxLayout;
//    hbl->addWidget(pi);
//    hbl->addLayout(vbl);
      ui->horizontalLayoutProgress->addWidget(pi,0,Qt::AlignCenter);
//    this->layout()->addWidget(pi);
}

void LostChekerDialog::finishGetInfo()
{
    qDebug() << "Finis" << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss.zzz");

    QSqlDatabase dba = QSqlDatabase::database("azs"+curTerminal.value("TERMINAL_ID").toString());
    modelShifts = new QSqlQueryModel();
    QString strSQL = "SELECT s.shift_id, s.znumber, s.datopen, s.datclose, s.isclose, o.fio from shifts s "
            "left JOIN operators o ON s.operator_id = o.operator_id "
            "order by s.shift_id desc";
    modelShifts->setQuery(strSQL, dba);
    while (modelShifts->canFetchMore())
          modelShifts->fetchMore();

    pi->stopAnimation();
    ui->horizontalLayoutProgress->removeWidget(pi);
    ui->frameProgress->hide();
    ui->groupBoxDataChek->show();
    ui->frame->setEnabled(false);

}

void LostChekerDialog::errogGetInfo(QString str)
{
    QMessageBox::critical(0, qApp->tr("Не могу открыть базу данных"),
                          QString("Не могу установить соединение с базой данных АЗС\nПричина: %1\n Проверьте наличее интернет соединения на АЗС.").arg(str),
                          QMessageBox::Ok);
}



void LostChekerDialog::on_lineEditShift_textChanged(const QString &arg1)
{
    QString strShift = "Смена не указана.";
    ui->labelShiftInfo->setText(strShift);
    for(int i=0;i<modelShifts->rowCount();++i){

        if(arg1.toInt() == modelShifts->data(modelShifts->index(i,0)).toInt()){
            strShift = "Открыта " + modelShifts->data(modelShifts->index(i,2)).toDateTime().toString("dd.MM.yyyy hh:mm:ss") +
                    " Z-отчет № " + modelShifts->data(modelShifts->index(i,1)).toString() +
                    " Оператор " + modelShifts->data(modelShifts->index(i,5)).toString();
            ui->labelShiftInfo->setText(strShift);
            break;
        }

    }
}

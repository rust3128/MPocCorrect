#include "deletesaleorderdialog.h"
#include "ui_deletesaleorderdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>
#include <QProgressDialog>
#include <QSqlRecord>
#include <QFile>


DeleteSaleorderDialog::DeleteSaleorderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteSaleorderDialog)
{
    ui->setupUi(this);
    clearUI();
    modelSale = new QSqlQueryModel();
}

DeleteSaleorderDialog::~DeleteSaleorderDialog()
{
    delete ui;
}

void DeleteSaleorderDialog::on_pushButtonFind_clicked()
{
    QSqlDatabase dbc = QSqlDatabase::database("central");
    QString strSql;
    QSqlQuery q = QSqlQuery(dbc);

//    QProgressDialog progress("Поиск транзакции", "Abort Copy", 0, 0);
//    progress.setWindowModality(Qt::ApplicationModal);
//    progress.setCancelButton(0);


    strSql=QString("SELECT a.TERMINAL_ID, t.name, a.SALEORDER_ID, a.NUM_CHECK,f.SHORTNAME, "
                   "a.GIVE, a.ORDERED, a.SUMMA, a.DISCOUNTSUMMA,a.GIVE1 "
                   "FROM SALEORDERS a "
                   "LEFT JOIN fuels f ON a.FUEL_ID=f.FUEL_ID "
                   "LEFT JOIN TERMINALS t ON a.TERMINAL_ID=t.TERMINAL_ID "
                   "where a.TERMINAL_ID=%1 and a.SHIFT_ID=%2 and a.NUM_CHECK=%3")
            .arg(ui->lineEditTerminal->text().toInt())
            .arg(ui->lineEditShift->text().toInt())
            .arg(ui->lineEditNumCheck->text().toInt());
//    progress.show();
    modelSale->setQuery(strSql,dbc);
//    QModelIndex index = modelSale->mimeTypes()


//    qApp->processEvents(QEventLoop::WaitForMoreEvents);
    if(modelSale->rowCount()==0){
        QMessageBox::critical(0, qApp->tr("Ошибка"),
                              QString("Транзакия с заданными параметрами отсутсвует в базе данных\n"
                                      "Проверьте информацю о транзакции и повторите попытку."));
        return;
    }
    ui->groupBoxOutput->show();
    strSql=QString("select a.DATOPEN from SHIFTS a "
                   "where a.TERMINAL_ID=%1 and a.SHIFT_ID=%2")
            .arg(ui->lineEditTerminal->text().toInt())
            .arg(ui->lineEditShift->text().toInt());
    q.exec(strSql);
    q.next();

    ui->labelAdress->setText(modelSale->data(modelSale->index(0,1)).toString().trimmed());
    ui->labelTerminal->setText("Терминал "+modelSale->data(modelSale->index(0,0)).toString().trimmed());
    ui->labelShift->setText("Смена № "+ ui->lineEditShift->text() + " от " + q.value(0).toDate().toString("dd MMMM yyyy"));
    modelSale->setHeaderData(3,Qt::Horizontal,"Чек");
    modelSale->setHeaderData(4,Qt::Horizontal,"Топливо");
    modelSale->setHeaderData(5,Qt::Horizontal,"Получено");
    modelSale->setHeaderData(6,Qt::Horizontal,"Заказано");
    modelSale->setHeaderData(7,Qt::Horizontal,"Сумма");
    modelSale->setHeaderData(8,Qt::Horizontal,"Скидка");


    ui->tableView->setModel(modelSale);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(9);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->resizeColumnsToContents();



}

void DeleteSaleorderDialog::clearUI()
{
    ui->lineEditNumCheck->clear();
    ui->lineEditShift->clear();
    ui->lineEditTerminal->clear();
    ui->labelAdress->clear();
    ui->labelTerminal->clear();
    ui->labelShift->clear();
    ui->groupBoxOutput->hide();
}

void DeleteSaleorderDialog::on_pushButtonUpdate_clicked()
{
    QSqlDatabase dbc = QSqlDatabase::database("central");
    QSqlQuery q = QSqlQuery(dbc);
//Формируем update для возможного отката
    QStringList buckUpdate;
    buckUpdate << "UPDATE SALEORDERS SET";
    buckUpdate << QString("GIVE = '%1'").arg(modelSale->data(modelSale->index(0,5)).toString().trimmed());
    buckUpdate << QString("ORDERED = '%1'").arg(modelSale->data(modelSale->index(0,6)).toDouble());
    buckUpdate << QString("SUMMA = '%1'").arg(modelSale->data(modelSale->index(0,7)).toString().trimmed());
    buckUpdate << QString("DISCOUNTSUMMA = '%1'").arg(modelSale->data(modelSale->index(0,8)).toString().trimmed());
    buckUpdate << QString("GIVE1 = '%1'").arg(modelSale->data(modelSale->index(0,8)).toDouble());
    buckUpdate << QString("WHERE TERMINAL_ID = '%1' "
                          "AND SALEORDER_ID = '%2';")
                  .arg(modelSale->data(modelSale->index(0,0)).toString().trimmed())
                  .arg(modelSale->data(modelSale->index(0,2)).toString().trimmed());

    QFile file("MPosCorrect.log");

    if (file.open(QIODevice::WriteOnly | QFile::Append ))
    {
        QTextStream stream(&file);
        stream << "Terminal " << ui->lineEditTerminal->text() << " Shift " << ui->lineEditShift->text()
               << " NumCheck " << ui->lineEditNumCheck->text() << " ";
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << endl;
        stream << "BUCKUP INFO" << endl;
        stream << "-------------------------------------------------------" << endl;
        for(int i=0;i<buckUpdate.size();++i){
            stream << buckUpdate.at(i)<<endl;
        }
        stream << "-------------------------------------------------------" << endl;

        file.close();
        if (stream.status() != QTextStream::Ok)
        {
            qDebug() << "Ошибка записи файла";
        }
    }
// Обнуляем запись.

    QString strSql = QString("UPDATE SALEORDERS SET "
                             "GIVE = '0.00', "
                             "ORDERED = '0.00', "
                             "SUMMA = '0.00', "
                             "DISCOUNTSUMMA = '0.00', "
                             "GIVE1 = '0.000' "
                             "WHERE TERMINAL_ID = '%1' AND SALEORDER_ID = '%2'")
            .arg(modelSale->data(modelSale->index(0,0)).toString().trimmed())
            .arg(modelSale->data(modelSale->index(0,2)).toString().trimmed());
//    qDebug() << strSql;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Подтверждение действия"),
                          QString::fromUtf8("Вы действительно хотите обнулить указанную транзакцию?"),
                          QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No){
        return;
    }


    if(!q.exec(strSql)) {
        QMessageBox::critical(0, qApp->tr("Ошибка"),
                              QString("Не возможно выблнить очистку транзакции\n"
                                      "Причина: %1"
                                      )
                              .arg(q.lastError().text()));
        return;
    }
    QMessageBox::information(0, qApp->tr("Операция завершена"),
                          QString("Очистка транзакции успешно произведена\n"));
    clearUI();

}

void DeleteSaleorderDialog::on_pushButton_2_clicked()
{
    clearUI();
}

void DeleteSaleorderDialog::on_pushButton_3_clicked()
{
    this->reject();
}

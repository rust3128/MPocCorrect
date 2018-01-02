#include "azsconnect.h"
#include <QDebug>
#include <QtSql>



AzsConnect::AzsConnect(QSqlRecord rec, QObject *parent) :  QObject(parent)
{
    azsRec=rec;

}

void AzsConnect::getAzsInfo()
{
    qDebug() << azsRec.value("SERVER_NAME").toString();
    ///Подключаемся к базе данных АЗС
    QString pass;
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE","azs"+azsRec.value("TERMINAL_ID").toString());
    db.setHostName(azsRec.value("SERVER_NAME").toString().trimmed());
    db.setDatabaseName(azsRec.value("DB_NAME").toString().trimmed());
    db.setUserName(azsRec.value("CON_LOGIN").toString().trimmed());

    if(azsRec.value("CON_PASSWORD").toString().trimmed() == "ncvxjxrm‚")
            pass="masterkey";
    if(azsRec.value("CON_PASSWORD").toString().trimmed() == "twqsnrhlv")
            pass="sunoiladm";
    db.setPassword(pass);
    if(!db.open()) {
        QString errorString =  db.lastError().text();
        qDebug() <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << errorString;
        emit connectionError(errorString);
        emit fin();

    }
    emit fin();


}

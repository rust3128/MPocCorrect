#ifndef AZSCONNECT_H
#define AZSCONNECT_H

#include <QObject>
#include <QSqlRecord>
#include <QSqlQueryModel>

class AzsConnect : public QObject
{
    Q_OBJECT
public:
    explicit AzsConnect(QSqlRecord rec, QObject *parent = nullptr);

signals:
    void connectionError(QString errorMeaage);
    void fin();
public slots:
    void getAzsInfo();

private:
    QSqlRecord azsRec;
    QSqlQueryModel *modelShifts;
};

#endif // AZSCONNECT_H

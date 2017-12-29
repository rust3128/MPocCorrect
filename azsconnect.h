#ifndef AZSCONNECT_H
#define AZSCONNECT_H

#include <QObject>
#include <QSqlRecord>

class AzsConnect : public QObject
{
    Q_OBJECT
public:
    explicit AzsConnect(QSqlRecord rec, QObject *parent = nullptr);

signals:

public slots:
    void getAzsInfo();

private:
    QSqlRecord azsRec;
};

#endif // AZSCONNECT_H

#ifndef LOSTCHEKERDIALOG_H
#define LOSTCHEKERDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "azsconnect.h"

namespace Ui {
class LostChekerDialog;
}

class LostChekerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LostChekerDialog(QWidget *parent = 0);
    ~LostChekerDialog();

private slots:
    void on_lineEditTerminal_textChanged(const QString &arg1);
    void on_pushButtonConnect_clicked();
    void startGetInfo();
    void finishGetInfo();

private:
    Ui::LostChekerDialog *ui;
    QSqlQueryModel *modelTerminals;
    QSqlRecord curTerminal;             //Запись о терминале в котроырй будем добавлять чек
    AzsConnect *dbAzs;
private:
    void createModelTerminals();
};

#endif // LOSTCHEKERDIALOG_H

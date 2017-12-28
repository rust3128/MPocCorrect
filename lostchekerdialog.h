#ifndef LOSTCHEKERDIALOG_H
#define LOSTCHEKERDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

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

private:
    Ui::LostChekerDialog *ui;
    QSqlQueryModel *modelTerminals;
private:
    void createModelTerminals();
};

#endif // LOSTCHEKERDIALOG_H

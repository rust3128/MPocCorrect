#ifndef DELETESALEORDERDIALOG_H
#define DELETESALEORDERDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class DeleteSaleorderDialog;
}

class DeleteSaleorderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteSaleorderDialog(QWidget *parent = 0);
    ~DeleteSaleorderDialog();

private slots:
    void on_pushButtonFind_clicked();

    void on_pushButtonUpdate_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    void clearUI();
private:
    Ui::DeleteSaleorderDialog *ui;
    QSqlQueryModel *modelSale;
};

#endif // DELETESALEORDERDIALOG_H

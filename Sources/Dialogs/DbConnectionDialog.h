#ifndef DBCONNECTIONDIALOG_H
#define DBCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class DbConnectionDialog;
}
//------------------------------------------------------------------------------
class DbConnectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DbConnectionDialog(QWidget *parent = 0);
    ~DbConnectionDialog();
    
private slots:
    void on_connectPushButton_clicked();

    void on_quitPushButton_clicked();

private:
    Ui::DbConnectionDialog *mUi;
};
//------------------------------------------------------------------------------
#endif // DBCONNECTIONDIALOG_H

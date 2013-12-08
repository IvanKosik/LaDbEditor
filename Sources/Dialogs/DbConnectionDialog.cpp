#include "DbConnectionDialog.h"
#include "ui_DbConnectionDialog.h"

#include "DB.h"

#include <QMessageBox>
//------------------------------------------------------------------------------
DbConnectionDialog::DbConnectionDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::DbConnectionDialog)
{
    mUi->setupUi(this);
}
//------------------------------------------------------------------------------
DbConnectionDialog::~DbConnectionDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
void DbConnectionDialog::on_connectPushButton_clicked()
{
    QString hostName = mUi->hostNameLineEdit->text();
    QString dbName = mUi->dbNameLineEdit->text();
    QString userName = mUi->userNameLineEdit->text();
    QString password = mUi->passwordLineEdit->text();
    QString port = mUi->portLineEdit->text();

    DB *db = DB::instance();
    // Если подключение к БД произведено успешно:
    if (db->openConnection(hostName, dbName, userName, password, port)) {
        done(QDialog::Accepted);
    } else {
        QString text = tr("%1\nError code: %2")
                .arg(db->lastErrorText(), QString::number(db->lastErrorCode()));
        QMessageBox::warning(this, QObject::tr("DB Connection Error"), text);
    }
}
//------------------------------------------------------------------------------
void DbConnectionDialog::on_quitPushButton_clicked()
{
    done(QDialog::Rejected);
}
//------------------------------------------------------------------------------

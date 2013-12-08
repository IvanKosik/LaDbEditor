#include "DiagnosisCreationDialog.h"
#include "ui_DiagnosisCreationDialog.h"

#include <QSqlError>
#include <QMessageBox>

#include <QDebug> //%
//------------------------------------------------------------------------------
DiagnosisCreationDialog::DiagnosisCreationDialog(QSqlTableModel *diagnosisModel,
                                                 QWidget *parent)
    : QDialog(parent), mUi(new Ui::DiagnosisCreationDialog),
      mDiagnosisModel(diagnosisModel)
{
    mUi->setupUi(this);
}
//------------------------------------------------------------------------------
DiagnosisCreationDialog::~DiagnosisCreationDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool DiagnosisCreationDialog::createDiagnosis()
{
    mDiagnosisModel->insertRow(mDiagnosisModel->rowCount());
    qint32 row = mDiagnosisModel->rowCount() - 1;
    if (!mDiagnosisModel->setData(mDiagnosisModel->index(row, 1),
                                  mUi->nameLineEdit->text())) {
        QSqlError lastError = mDiagnosisModel->lastError();
        QString text(tr("Cannot insert the values in the model!\nError code: %1\nError text: %2")
                    .arg(QString::number(lastError.number()), lastError.text()));
        QMessageBox::warning(this, tr("Error"), text);
        return false;
    }
    mDiagnosisModel->submitAll();

    return true;
}
//------------------------------------------------------------------------------
void DiagnosisCreationDialog::on_nameLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        mUi->okPushButton->setDisabled(true);
        mUi->createPushButton->setDisabled(true);
    } else {
        mUi->okPushButton->setDisabled(false);
        mUi->createPushButton->setDisabled(false);
    }
}
//------------------------------------------------------------------------------
void DiagnosisCreationDialog::on_okPushButton_clicked()
{
    if (createDiagnosis()) {
        close();
    }
}
//------------------------------------------------------------------------------
void DiagnosisCreationDialog::on_createPushButton_clicked()
{
    if (createDiagnosis()) {
        mUi->nameLineEdit->clear();
        mUi->nameLineEdit->setFocus();
    }
}
//------------------------------------------------------------------------------
void DiagnosisCreationDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------

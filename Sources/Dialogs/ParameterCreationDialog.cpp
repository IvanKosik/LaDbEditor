#include "ParameterCreationDialog.h"
#include "ui_ParameterCreationDialog.h"

#include "ParameterAddingDialog.h"

#include <QMessageBox>
//------------------------------------------------------------------------------
ParameterCreationDialog::ParameterCreationDialog(QSqlTableModel *parameterModel,
                                                 QWidget *parent)
    : QDialog(parent), mUi(new Ui::ParameterCreationDialog),
      mParameterModel(parameterModel)
{
    mUi->setupUi(this);
}
//------------------------------------------------------------------------------
ParameterCreationDialog::~ParameterCreationDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool ParameterCreationDialog::createParameter()
{
    mParameterModel->insertRow(mParameterModel->rowCount());
    qint32 row = mParameterModel->rowCount() - 1;
    if (!mParameterModel->setData(mParameterModel->index(row, 1),
                                  mUi->nameLineEdit->text())) {
        QSqlError lastError = mParameterModel->lastError();
        QString text(tr("Cannot insert the values in the model!\nError code: %1\nError text: %2")
                    .arg(QString::number(lastError.number()), lastError.text()));
        QMessageBox::warning(this, tr("Error"), text);
        return false;
    }
    mParameterModel->submitAll();

    return true;
}
//------------------------------------------------------------------------------
void ParameterCreationDialog::on_nameLineEdit_textChanged(const QString &arg1)
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
void ParameterCreationDialog::on_okPushButton_clicked()
{
    if (createParameter()) {
        close();
    }
}
//------------------------------------------------------------------------------
void ParameterCreationDialog::on_createPushButton_clicked()
{
    if (createParameter()) {
        mUi->nameLineEdit->clear();
        mUi->nameLineEdit->setFocus();
    }
}
//------------------------------------------------------------------------------
void ParameterCreationDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------

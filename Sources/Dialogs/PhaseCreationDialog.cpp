#include "PhaseCreationDialog.h"
#include "ui_PhaseCreationDialog.h"

#include "MainWindow.h"

#include <QSqlError>
#include <QMessageBox>

#include <QDebug> //%
//------------------------------------------------------------------------------
PhaseCreationDialog::PhaseCreationDialog(QSqlTableModel *phaseModel,
                                         QWidget *parent)
    : QDialog(parent), mUi(new Ui::PhaseCreationDialog),
      mPhaseModel(phaseModel)
{
    mUi->setupUi(this);
}
//------------------------------------------------------------------------------
PhaseCreationDialog::~PhaseCreationDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool PhaseCreationDialog::createPhase()
{
    MainWindow *mainWindow = (MainWindow *)parentWidget();
    // Если выделен какой-либо диагноз:
    if (mainWindow->hasSelectedDiagnosis()) {
        mPhaseModel->insertRow(mPhaseModel->rowCount());
        qint32 row = mPhaseModel->rowCount() - 1;
        mPhaseModel->setData(mPhaseModel->index(row, 1),
                             mainWindow->selectedDiagnosisId());
        if (!mPhaseModel->setData(mPhaseModel->index(row, 2),
                                  mUi->nameLineEdit->text())) {
            QSqlError lastError = mPhaseModel->lastError();
            QString text(tr("Cannot insert the values in the model!\nError code: %1\nError text: %2")
                        .arg(QString::number(lastError.number()), lastError.text()));
            QMessageBox::warning(this, tr("Error"), text);
            return false;
        }
        mPhaseModel->submitAll();
        mainWindow->setWasPhasesChanged(true);
    } else {
        QMessageBox::information(this, tr("Create Phase"),
                                 tr("Select at least one diagnosis record."));
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void PhaseCreationDialog::on_nameLineEdit_textChanged(const QString &arg1)
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
void PhaseCreationDialog::on_okPushButton_clicked()
{
    if (createPhase()) {
        close();
    }
}
//------------------------------------------------------------------------------
void PhaseCreationDialog::on_createPushButton_clicked()
{
    if (createPhase()) {
        mUi->nameLineEdit->clear();
        mUi->nameLineEdit->setFocus();
    }
}
//------------------------------------------------------------------------------
void PhaseCreationDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------

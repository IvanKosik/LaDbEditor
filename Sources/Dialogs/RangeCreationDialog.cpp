#include "RangeCreationDialog.h"
#include "ui_RangeCreationDialog.h"

#include "MainWindow.h"

#include <QMessageBox>
//------------------------------------------------------------------------------
RangeCreationDialog::RangeCreationDialog(QSqlTableModel *rangeModel, QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::RangeCreationDialog), mRangeModel(rangeModel)
{
    mUi->setupUi(this);
}
//------------------------------------------------------------------------------
RangeCreationDialog::~RangeCreationDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool RangeCreationDialog::createRange()
{
    MainWindow *mainWindow = (MainWindow *)parentWidget();
    // Если выделен какой-либо параметр:
    if (mainWindow->hasSelectedParameter()) {
        Id phaseId = mainWindow->selectedPhaseId();
        Id parameterId = mainWindow->selectedParameterId();
        double rangeBegin = mUi->rangeBeginLineEdit->text().toDouble();
        double rangeEnd = mUi->rangeEndLineEdit->text().toDouble();
        double beginProbability = mUi->beginProbabilityLineEdit->text().toDouble();
        double endProbability = mUi->endProbabilityLineEdit->text().toDouble();
        /*qint32 rangeBegin = mUi->rangeBeginLineEdit->text().toInt();
        qint32 rangeEnd = mUi->rangeEndLineEdit->text().toInt();
        qint32 beginProbability = mUi->beginProbabilityLineEdit->text().toInt();
        qint32 endProbability = mUi->endProbabilityLineEdit->text().toInt();*/
        QString description = mUi->descriptionTextEdit->toPlainText();
        DB::addPhaseParameterRange(phaseId, parameterId, rangeBegin, rangeEnd,
                                   beginProbability, endProbability, description);

        // Обновляем таблицу диапазонов параметра:
        mRangeModel->select();
    } else {
        QMessageBox::information(this, tr("Create Range"),
                                 tr("Select at least one parameter record."));
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void RangeCreationDialog::disableAcceptButtons(bool disable)
{
    if (disable) {
        mUi->okPushButton->setDisabled(true);
        mUi->createPushButton->setDisabled(true);
    } else {
        if (!mUi->rangeBeginLineEdit->text().isEmpty()
                && !mUi->rangeEndLineEdit->text().isEmpty()
                && !mUi->beginProbabilityLineEdit->text().isEmpty()
                && !mUi->endProbabilityLineEdit->text().isEmpty()) {
            mUi->okPushButton->setDisabled(false);
            mUi->createPushButton->setDisabled(false);
        }
    }
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_okPushButton_clicked()
{
    if (createRange()) {
        close();
    }
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_createPushButton_clicked()
{
    if (createRange()) {
        mUi->rangeBeginLineEdit->clear();
        mUi->rangeEndLineEdit->clear();
        mUi->beginProbabilityLineEdit->clear();
        mUi->endProbabilityLineEdit->clear();
        mUi->descriptionTextEdit->clear();

        mUi->rangeBeginLineEdit->setFocus();
    }
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_rangeBeginLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        disableAcceptButtons(true);
    } else {
        disableAcceptButtons(false);
    }
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_rangeEndLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        disableAcceptButtons(true);
    } else {
        disableAcceptButtons(false);
    }
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_beginProbabilityLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        disableAcceptButtons(true);
    } else {
        disableAcceptButtons(false);
    }
}
//------------------------------------------------------------------------------
void RangeCreationDialog::on_endProbabilityLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        disableAcceptButtons(true);
    } else {
        disableAcceptButtons(false);
    }
}
//------------------------------------------------------------------------------

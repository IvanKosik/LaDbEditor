#include "ResearchCreationDialog.h"
#include "ui_ResearchCreationDialog.h"

#include <QMessageBox>
#include <QSqlError>
//------------------------------------------------------------------------------
ResearchCreationDialog::ResearchCreationDialog(QSqlTableModel *researchModel,
                                               QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::ResearchCreationDialog), mResearchModel(researchModel)
{
    mUi->setupUi(this);
}
//------------------------------------------------------------------------------
ResearchCreationDialog::~ResearchCreationDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool ResearchCreationDialog::createResearch()
{
    mResearchModel->insertRow(mResearchModel->rowCount());
    qint32 row = mResearchModel->rowCount() - 1;
    if (!mResearchModel->setData(mResearchModel->index(row, 1),
                                  mUi->nameLineEdit->text())) {
        QSqlError lastError = mResearchModel->lastError();
        QString text(tr("Cannot insert the values in the model!\nError code: %1\nError text: %2")
                    .arg(QString::number(lastError.number()), lastError.text()));
        QMessageBox::warning(this, tr("Error"), text);
        return false;
    }
    mResearchModel->submitAll();

    return true;
}
//------------------------------------------------------------------------------
void ResearchCreationDialog::on_nameLineEdit_textChanged(const QString &arg1)
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
void ResearchCreationDialog::on_okPushButton_clicked()
{
    if (createResearch()) {
        close();
    }
}
//------------------------------------------------------------------------------
void ResearchCreationDialog::on_createPushButton_clicked()
{
    if (createResearch()) {
        mUi->nameLineEdit->clear();
        mUi->nameLineEdit->setFocus();
    }
}
//------------------------------------------------------------------------------
void ResearchCreationDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------

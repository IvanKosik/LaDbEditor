#include "ResearchAddingDialog.h"
#include "ui_ResearchAddingDialog.h"

#include "ParameterAddingDialog.h"

#include <QMessageBox>

const QString ResearchTableName = "Research";
//------------------------------------------------------------------------------
ResearchAddingDialog::ResearchAddingDialog(QSqlTableModel *parameterResearchModel,
                                           QWidget *parent) :
    QDialog(parent), mUi(new Ui::ResearchAddingDialog),
    mParameterResearchModel(parameterResearchModel), mResearchCreationDialog(0)
{
    mUi->setupUi(this);

    mResearchModel = new QSqlTableModel;
    mResearchModel->setTable(ResearchTableName);
    mResearchModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mResearchModel->select();
    mResearchModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mResearchModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    connect(mResearchModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(researchChanged(QModelIndex,QModelIndex)));

//    mResearchProxyModel = new QSortFilterProxyModel;
//    mResearchProxyModel->setSourceModel(mResearchModel);
    mUi->researchTableView->setModel(mResearchModel);
    mUi->researchTableView->hideColumn(0);
    mUi->researchTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
//------------------------------------------------------------------------------
ResearchAddingDialog::~ResearchAddingDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool ResearchAddingDialog::addSelectedResearches()
{
    ParameterAddingDialog *parameterAddingDialog
            = (ParameterAddingDialog *)parentWidget();
    // Если выделен какой-либо параметр:
    if (parameterAddingDialog->hasSelectedParameter()) {
        Id selectedParameterId = parameterAddingDialog->selectedParameterId();
        // Если выделено хотя бы одно исследование:
        if (!mUi->researchTableView->selectionModel()->selectedRows().isEmpty()) {
            foreach (QModelIndex researchIdIndex,
                     mUi->researchTableView->selectionModel()->selectedRows()) {
                Id researchId = researchIdIndex.data().toULongLong();
                DB::addParameterResearch(selectedParameterId, researchId);
            }
            // Обновляем таблицу исследований параметра:
            mParameterResearchModel->select();
        } else {
            QMessageBox::information(this, tr("Add Researches"),
                                     tr("Select at least one research record."));
            return false;
        }
    } else {
        QMessageBox::information(this, tr("Add Researches"),
                                 tr("Select at least one parameter record."));
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::on_createResearchPushButton_clicked()
{
    if (mResearchCreationDialog == 0) {
        mResearchCreationDialog = new ResearchCreationDialog(mResearchModel, this);
        connect(mResearchCreationDialog, SIGNAL(finished(int)),
                SLOT(researchCreationDialogFinished(int)));
        mResearchCreationDialog->show();
    } else {
        mResearchCreationDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::on_deleteResearchPushButton_clicked()
{
    // Если выделено хотя бы одно исследование:
    if (!mUi->researchTableView->selectionModel()->selectedIndexes().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Delete Researches?"),
                tr("Are you sure you want to delete the selected research records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedIndex,
                     mUi->researchTableView->selectionModel()->selectedIndexes()) {
                mResearchModel->removeRow(selectedIndex.row());
            }
            mResearchModel->submitAll();
            // Обновляем таблицу исследований параметра:
            mParameterResearchModel->select();
        }
    }
    else {
        QMessageBox::information(this, tr("Delete Researches"),
                                 tr("Select at least one research record."));
    }
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::researchCreationDialogFinished(int)
{
    mResearchCreationDialog->deleteLater();
    mResearchCreationDialog = 0;
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::researchChanged(QModelIndex, QModelIndex)
{
    mResearchModel->submitAll();
    mParameterResearchModel->select();
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::on_okPushButton_clicked()
{
    if (addSelectedResearches()) {
        close();
    }
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::on_addPushButton_clicked()
{
    addSelectedResearches();
}
//------------------------------------------------------------------------------
void ResearchAddingDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------

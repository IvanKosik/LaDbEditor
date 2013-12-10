#include "ParameterAddingDialog.h"
#include "ui_ParameterAddingDialog.h"

#include "MainWindow.h"

#include <QMessageBox>

const QString ParameterTableName = "Parameter";
const QString ResearchTableName = "Research";
//------------------------------------------------------------------------------
ParameterAddingDialog::ParameterAddingDialog(QWidget *parent)
    : QDialog(parent), mUi(new Ui::ParameterAddingDialog)
    , mHasSelectedParameter(false), mParameterCreationDialog(0)
    , mResearchAddingDialog(0)
{
    mUi->setupUi(this);

    mParameterModel = new QSqlTableModel;
    mParameterModel->setTable(ParameterTableName);
    mParameterModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mParameterModel->select();
    mParameterModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mParameterModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    connect(mParameterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(parameterChanged(QModelIndex,QModelIndex)));

//    mParameterProxyModel = new QSortFilterProxyModel;
//    mParameterProxyModel->setSourceModel(mParameterModel);
    mUi->parameterTableView->setModel(mParameterModel);
    mUi->parameterTableView->hideColumn(0);
    mUi->parameterTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    connect(mUi->parameterTableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(currentParameterChanged(QModelIndex, QModelIndex)));


    mResearchModel = new QSqlTableModel;
    mResearchModel->setTable(ResearchTableName);
    mResearchModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mResearchModel->select();
    mResearchModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mResearchModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

//    mResearchProxyModel = new QSortFilterProxyModel;
//    mResearchProxyModel->setSourceModel(mResearchModel);
    mUi->researchTableView->setModel(mResearchModel);
    mUi->researchTableView->hideColumn(0);
    mUi->researchTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
//------------------------------------------------------------------------------
ParameterAddingDialog::~ParameterAddingDialog()
{
    delete mUi;
}
//------------------------------------------------------------------------------
bool ParameterAddingDialog::hasSelectedParameter() const
{
    return mHasSelectedParameter;
}
//------------------------------------------------------------------------------
Id ParameterAddingDialog::selectedParameterId() const
{
    return mSelectedParameterId;
}
//------------------------------------------------------------------------------
bool ParameterAddingDialog::addSelectedParameters()
{
    MainWindow *mainWindow = (MainWindow *)parentWidget();
    // Если выделена какая-либо фаза:
    if (mainWindow->hasSelectedPhase()) {
        Id selectedPhaseId = mainWindow->selectedPhaseId();
        // Если выделен хотя бы один параметр:
        if (!mUi->parameterTableView->selectionModel()->selectedRows().isEmpty()) {
            foreach (QModelIndex parameterIdIndex,
                     mUi->parameterTableView->selectionModel()->selectedRows()) {
                Id parameterId = parameterIdIndex.data().toULongLong();
                DB::addPhaseParameter(selectedPhaseId, parameterId);
            }
            // Обновляем таблицу параметров фазы:
            MainWindow::updatePhaseParameters();
            //%mPhaseParameterModel->select();
        } else {
            QMessageBox::information(this, tr("Add Parameters"),
                                     tr("Select at least one parameter record."));
            return false;
        }
    } else {
        QMessageBox::information(this, tr("Add Parameters"),
                                 tr("Select at least one phase record."));
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::currentParameterChanged(QModelIndex current, QModelIndex)
{
    // Если выделен хотя бы один параметр:
    if (current.isValid()) {
        mHasSelectedParameter = true;
        // Получаем идентификатор текущего выделенного параметра:
        mSelectedParameterId = mParameterModel->index(current.row(), 0).data().toULongLong();
        // Отображаем только те исследования, которые позволяют определить выделенный параметр:
        mResearchModel->setFilter("ID IN (SELECT ResearchID FROM ParameterResearch WHERE ParameterID="
                                   + QString::number(mSelectedParameterId) + ")");
//        mWasParametersChanged = false;
    } else {
        mHasSelectedParameter = false;
        // Не отображаем ни одного исследования:
        mResearchModel->setFilter("0=1");
//        mPhaseModel->clear(); //%Ведь нет выделенного диагноза
    }
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::parameterChanged(QModelIndex, QModelIndex)
{
    mParameterModel->submitAll();
    // Обновляем таблицу параметров фазы:
    //%mPhaseParameterModel->select();
    MainWindow::updatePhaseParameters();
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_createParameterPushButton_clicked()
{
    if (mParameterCreationDialog == 0) {
        mParameterCreationDialog = new ParameterCreationDialog(mParameterModel, this);
        connect(mParameterCreationDialog, SIGNAL(finished(int)),
                SLOT(parameterCreationDialogFinished(int)));
        mParameterCreationDialog->show();
    } else {
        mParameterCreationDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_deleteParameterPushButton_clicked()
{
    // Если выделен хотя бы один параметр:
    if (!mUi->parameterTableView->selectionModel()->selectedIndexes().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Delete Parameters?"),
                tr("Are you sure you want to delete the selected parameter records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedIndex,
                     mUi->parameterTableView->selectionModel()->selectedIndexes()) {
                mParameterModel->removeRow(selectedIndex.row());
            }
            mParameterModel->submitAll();
        }
    }
    else {
        QMessageBox::information(this, tr("Delete Parameters"),
                                 tr("Select at least one parameter record."));
    }
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::parameterCreationDialogFinished(int)
{
    mParameterCreationDialog->deleteLater();
    mParameterCreationDialog = 0;
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_addResearchPushButton_clicked()
{
    if (mResearchAddingDialog == 0) {
        mResearchAddingDialog = new ResearchAddingDialog(mResearchModel, this);
        connect(mResearchAddingDialog, SIGNAL(finished(int)),
                SLOT(researchAddingDialogFinished(int)));
        mResearchAddingDialog->show();
    } else {
        mResearchAddingDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_removeResearchPushButton_clicked()
{
    // Если выделено хотя бы одно исследование:
    if (!mUi->researchTableView->selectionModel()->selectedRows().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Remove Researches?"),
                tr("Are you sure you want to remove the selected research records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedResearchIndex,
                     mUi->researchTableView->selectionModel()->selectedRows()) {
                Id researchId = mResearchModel->index(selectedResearchIndex.row(), 0)
                        .data().toULongLong();
                DB::deleteParameterResearch(mSelectedParameterId, researchId);
            }
            mResearchModel->select();
        }
    } else {
        QMessageBox::information(this, tr("Remove Researches"),
                                 tr("Select at least one research record."));
    }
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::researchAddingDialogFinished(int)
{
    mResearchAddingDialog->deleteLater();
    mResearchAddingDialog = 0;
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_okPushButton_clicked()
{
    if (addSelectedParameters()) {
        close();
    }
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_addPushButton_clicked()
{
    addSelectedParameters();
}
//------------------------------------------------------------------------------
void ParameterAddingDialog::on_cancelPushButton_clicked()
{
    close();
}
//------------------------------------------------------------------------------

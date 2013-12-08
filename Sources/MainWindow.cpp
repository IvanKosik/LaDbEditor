#include "MainWindow.h"
#include "ui_MainWindow.h"

//#include "ItemSelectionModels/DiagnosisItemSelectionModel.h"

#include <QSortFilterProxyModel>
#include <SqlQuery.h>

#include <QDebug> //%
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

const QString DiagnosisTableName = "Diagnosis";
const QString PhaseTableName = "Phase";
const QString ParameterTableName = "Parameter";
const QString RangeTableName = "PhaseParameterRange";
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mUi(new Ui::MainWindow), mDiagnosisCreationDialog(0),
      mPhaseCreationDialog(0), mParameterAddingDialog(0), mHasSelectedDiagnosis(false),
      mWasPhasesChanged(false), mHasSelectedPhase(false), mHasSelectedParameter(false),
      mRangeCreationDialog(0)
{
    mUi->setupUi(this);

    createModels();


//    mDiagnosisProxyModel = new QSortFilterProxyModel;
//    mDiagnosisProxyModel->setSourceModel(mDiagnosisModel);
    mUi->diagnosisTableView->setModel(mDiagnosisModel);
    mUi->diagnosisTableView->hideColumn(0);
    mUi->diagnosisTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

//    DiagnosisItemSelectionModel *diagnosisItemSelectionModel
//            = new DiagnosisItemSelectionModel(mDiagnosisProxyModel, this); //-mDiagnosisModel
//    mUi->diagnosisTableView->setSelectionModel(diagnosisItemSelectionModel);
    connect(mUi->diagnosisTableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(currentDiagnosisChanged(QModelIndex, QModelIndex)));


//    mPhaseProxyModel = new QSortFilterProxyModel;
//    mPhaseProxyModel->setSourceModel(mPhaseModel);
    mUi->phaseTableView->setModel(mPhaseModel);
    mUi->phaseTableView->hideColumn(0);
    mUi->phaseTableView->hideColumn(1);
    mUi->phaseTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    connect(mUi->phaseTableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(currentPhaseChanged(QModelIndex, QModelIndex)));


//    mParameterProxyModel = new QSortFilterProxyModel;
//    mParameterProxyModel->setSourceModel(mParameterModel);
    mUi->parameterTableView->setModel(mParameterModel);
    mUi->parameterTableView->hideColumn(0);
    mUi->parameterTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    connect(mUi->parameterTableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(currentParameterChanged(QModelIndex, QModelIndex)));


//    mRangeProxyModel = new QSortFilterProxyModel;
//    mRangeProxyModel->setSourceModel(mRangeModel);
    mUi->rangeTableView->setModel(mRangeModel);
    mUi->rangeTableView->hideColumn(0);
    mUi->rangeTableView->hideColumn(1);
    mUi->rangeTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete mUi;

    //%delete proxyModel;
}
//------------------------------------------------------------------------------
bool MainWindow::hasSelectedDiagnosis() const
{
    return mHasSelectedDiagnosis;
}
//------------------------------------------------------------------------------
Id MainWindow::selectedDiagnosisId() const
{
    return mSelectedDiagnosisId;
}
//------------------------------------------------------------------------------
bool MainWindow::hasSelectedPhase() const
{
    return mHasSelectedPhase;
}
//------------------------------------------------------------------------------
Id MainWindow::selectedPhaseId() const
{
    return mSelectedPhaseId;
}
//------------------------------------------------------------------------------
bool MainWindow::hasSelectedParameter() const
{
    return mHasSelectedParameter;
}
//------------------------------------------------------------------------------
Id MainWindow::selectedParameterId() const
{
    return mSelectedParameterId;
}
//------------------------------------------------------------------------------
void MainWindow::setWasPhasesChanged(bool wasPhasesChanged)
{
    mWasPhasesChanged = wasPhasesChanged;
}
//------------------------------------------------------------------------------
bool MainWindow::wasPhasesChanged() const
{
    return mWasPhasesChanged;
}
//------------------------------------------------------------------------------
void MainWindow::createModels()
{
    createDiagnosisModel();
    createPhaseModel();
    createParameterModel();
    createRangeModel();
}
//------------------------------------------------------------------------------
void MainWindow::createDiagnosisModel()
{
    mDiagnosisModel = new QSqlTableModel;
    mDiagnosisModel->setTable(DiagnosisTableName);
    mDiagnosisModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mDiagnosisModel->select();
    mDiagnosisModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mDiagnosisModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    connect(mDiagnosisModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(diagnosisChanged(QModelIndex, QModelIndex)));
}
//------------------------------------------------------------------------------
void MainWindow::createPhaseModel()
{
    mPhaseModel = new QSqlTableModel;
    mPhaseModel->setTable(PhaseTableName);
    mPhaseModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mPhaseModel->select();
    mPhaseModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mPhaseModel->setHeaderData(1, Qt::Horizontal, tr("Diagnosis ID"));
    mPhaseModel->setHeaderData(2, Qt::Horizontal, tr("Name"));
    connect(mPhaseModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(phaseChanged(QModelIndex,QModelIndex)));
}
//------------------------------------------------------------------------------
void MainWindow::createParameterModel()
{
    mParameterModel = new QSqlTableModel;
    mParameterModel->setTable(ParameterTableName);
    mParameterModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mParameterModel->select();
    mParameterModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mParameterModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    connect(mParameterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(parameterChanged(QModelIndex,QModelIndex)));
}
//------------------------------------------------------------------------------
void MainWindow::createRangeModel()
{
    mRangeModel = new QSqlTableModel();
    mRangeModel->setTable(RangeTableName);
    mRangeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mRangeModel->select();
    mRangeModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    mRangeModel->setHeaderData(1, Qt::Horizontal, tr("Phase Parameter ID"));
    mRangeModel->setHeaderData(2, Qt::Horizontal, tr("Range Begin"));
    mRangeModel->setHeaderData(3, Qt::Horizontal, tr("Range End"));
    mRangeModel->setHeaderData(4, Qt::Horizontal, tr("Begin Probability"));
    mRangeModel->setHeaderData(5, Qt::Horizontal, tr("End Probability"));
    mRangeModel->setHeaderData(6, Qt::Horizontal, tr("Description"));
    connect(mRangeModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(rangeChanged(QModelIndex,QModelIndex)));
}
//------------------------------------------------------------------------------
void MainWindow::savePhaseChanges()
{
/*    QSqlDatabase db = mPhaseModel->database(); //% Получить объект из класса DB, а не так.
    db.transaction();
    if (mPhaseModel->submitAll()) {
        db.commit();
    } else {
        db.rollback();

        QSqlError lastError = mPhaseModel->lastError();
        QString text(tr("The database reported an error: %1").arg(lastError.text()));
        QMessageBox::warning(this, tr("Error"), text);
    }*/

    mPhaseModel->submitAll();

    //% Тут вызывать методы дальше по цепочке (для параметров) (а у параметров - для диапазонов)

    mWasPhasesChanged = false;
}
//------------------------------------------------------------------------------
void MainWindow::on_createDiagnosisPushButton_clicked()
{
    if (mDiagnosisCreationDialog == 0) {
        mDiagnosisCreationDialog = new DiagnosisCreationDialog(mDiagnosisModel, this);
        connect(mDiagnosisCreationDialog, SIGNAL(finished(int)),
                SLOT(diagnosisCreationDialogFinished(int)));
        mDiagnosisCreationDialog->show();
    } else {
        mDiagnosisCreationDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
/*void MainWindow::on_submitPushButton_clicked()
{
    Изменяю фазы. Жму сабмит. ошибка.. т.к. тут пользуюсь моделью диагнозов???
    QSqlDatabase db = mDiagnosisModel->database(); //% Получить объект из класса DB, а не так.
    db.transaction();
    if (mDiagnosisModel->submitAll()) {
        db.commit();
    } else {
        db.rollback();

        QSqlError lastError = mDiagnosisModel->lastError();
        QString text(tr("The database reported an error: %1").arg(lastError.text()));
        QMessageBox::warning(this, tr("Error"), text);
    }


//    savePhaseChanges();

    mPhaseModel->submitAll();

//    mDiagnosisModel->submitAll();
    //%Остальным моделям тоже вызвать этот метод.
}
//------------------------------------------------------------------------------
void MainWindow::on_revertPushButton_clicked()
{
    mDiagnosisModel->revertAll();
    mPhaseModel->revertAll();
    //%Остальным моделям тоже вызвать этот метод.
}*/
//------------------------------------------------------------------------------
void MainWindow::on_quitPushButton_clicked()
{
    qApp->quit();
}
//------------------------------------------------------------------------------
void MainWindow::diagnosisCreationDialogFinished(int)
{
    mDiagnosisCreationDialog->deleteLater();
    mDiagnosisCreationDialog = 0;
}
//------------------------------------------------------------------------------
void MainWindow::currentDiagnosisChanged(QModelIndex current, QModelIndex previous)
{
    // Если выделен хотя бы один диагноз:
    if (current.isValid()) {
        // Если фазы были изменены:
//        if (mWasPhasesChanged) {
//            savePhaseChanges();
//        }
        mHasSelectedDiagnosis = true;
        // Получаем идентификатор текущего выделенного диагноза:
        mSelectedDiagnosisId = mDiagnosisModel->index(current.row(), 0).data().toULongLong();
        // Отображаем только те фазы, которые принадлежат выделенному диагнозу:
        mPhaseModel->setFilter("DiagnosisID=" + QString::number(mSelectedDiagnosisId));
        mWasPhasesChanged = false;
    } else {
        mHasSelectedDiagnosis = false;
        // Не отображаем ни одной фазы:
        mPhaseModel->setFilter("0=1");
//%        mPhaseModel->clear(); //%Ведь нет выделенного диагноза
    }

    // Чтобы обновить модель параметров:
    if (mPhaseModel->rowCount() > 0) {
        mUi->phaseTableView->selectRow(0);
    } else {
        currentPhaseChanged(mPhaseModel->index(0, 0), mPhaseModel->index(0, 0)); //%!
    }
}
//------------------------------------------------------------------------------
void MainWindow::diagnosisChanged(QModelIndex, QModelIndex)
{
    mDiagnosisModel->submitAll();
}
//------------------------------------------------------------------------------
void MainWindow::on_deleteDiagnosisPushButton_clicked()
{
    // Если выделен хотя бы один диагноз:
    if (!mUi->diagnosisTableView->selectionModel()->selectedIndexes().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Delete Diagnoses?"),
                tr("Are you sure you want to delete the selected diagnosis records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedIndex,
                     mUi->diagnosisTableView->selectionModel()->selectedIndexes()) {
                mDiagnosisModel->removeRow(selectedIndex.row());
            }
            mDiagnosisModel->submitAll();
        }
    }
    else {
        QMessageBox::information(this, tr("Delete Diagnoses"),
                                 tr("Select at least one diagnosis record."));
    }
}
//------------------------------------------------------------------------------
void MainWindow::on_createPhasePushButton_clicked()
{
    if (mPhaseCreationDialog == 0) {
        mPhaseCreationDialog = new PhaseCreationDialog(mPhaseModel, this);
        connect(mPhaseCreationDialog, SIGNAL(finished(int)),
                SLOT(phaseCreationDialogFinished(int)));
        mPhaseCreationDialog->show();
    } else {
        mPhaseCreationDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
void MainWindow::on_deletePhasePushButton_clicked()
{
    // Если выделена хотя бы одна фаза:
    if (!mUi->phaseTableView->selectionModel()->selectedIndexes().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Delete Phases?"),
                tr("Are you sure you want to delete the selected phase records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedIndex,
                     mUi->phaseTableView->selectionModel()->selectedIndexes()) {
                mPhaseModel->removeRow(selectedIndex.row());
            }
            mPhaseModel->submitAll();
        }
    }
    else {
        QMessageBox::information(this, tr("Delete Phases"),
                                 tr("Select at least one phase record."));
    }
}
//------------------------------------------------------------------------------
void MainWindow::phaseCreationDialogFinished(int)
{
    mPhaseCreationDialog->deleteLater();
    mPhaseCreationDialog = 0;
}
//------------------------------------------------------------------------------
void MainWindow::currentPhaseChanged(QModelIndex current, QModelIndex previous)
{
    // Если выделена хотя бы одна фаза:
    if (current.isValid()) {
        mHasSelectedPhase = true;
        // Получаем идентификатор текущей выделенной фазы:
        mSelectedPhaseId = mPhaseModel->index(current.row(), 0).data().toULongLong();
        // Отображаем только те параметры, которые принадлежат выделенной фазе:
        mParameterModel->setFilter("ID IN (SELECT ParameterID FROM PhaseParameter WHERE PhaseID="
                                   + QString::number(mSelectedPhaseId) + ")");
//        mWasParametersChanged = false;
    } else {
        mHasSelectedPhase = false;
        // Не отображаем ни одного параметра:
        mParameterModel->setFilter("0=1");
//        mPhaseModel->clear(); //%Ведь нет выделенного диагноза
    }

    // Чтобы обновить модель диапазонов:
    if (mParameterModel->rowCount() > 0) {
        mUi->parameterTableView->selectRow(0);
    } else {
        currentParameterChanged(mParameterModel->index(0, 0), mParameterModel->index(0, 0)); //%!
    }
}
//------------------------------------------------------------------------------
void MainWindow::phaseChanged(QModelIndex, QModelIndex)
{
    mPhaseModel->submitAll();
}
//------------------------------------------------------------------------------
void MainWindow::currentParameterChanged(QModelIndex current, QModelIndex previous)
{
    // Если выделен хотя бы один параметр:
    if (current.isValid()) {
        mHasSelectedParameter = true;
        // Получаем идентификатор текущего выделенного параметра:
        mSelectedParameterId = mParameterModel->index(current.row(), 0).data().toULongLong();
        // Отображаем только те диапазоны, которые принадлежат выделенному параметру:
        // Получим PhaseParameterID:
        Id phaseParameterId
                = DB::getPhaseParameterId(mSelectedPhaseId, mSelectedParameterId);
        mRangeModel->setFilter("PhaseParameterID=" + QString::number(phaseParameterId));
//        mWasParametersChanged = false;
    } else {
        mHasSelectedParameter = false;
        // Не отображаем ни одного диапазона:
        mRangeModel->setFilter("0=1");
//        mPhaseModel->clear(); //%Ведь нет выделенного диагноза
    }
}
//------------------------------------------------------------------------------
void MainWindow::parameterChanged(QModelIndex, QModelIndex)
{
    mParameterModel->submitAll();
}
//------------------------------------------------------------------------------
void MainWindow::on_addParameterPushButton_clicked()
{
    if (mParameterAddingDialog == 0) {
        mParameterAddingDialog = new ParameterAddingDialog(mParameterModel, this);
        connect(mParameterAddingDialog, SIGNAL(finished(int)),
                SLOT(parameterAddingDialogFinished(int)));
        mParameterAddingDialog->show();
    } else {
        mParameterAddingDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
void MainWindow::on_removeParameterPushButton_clicked()
{
    // Если выделен хотя бы один параметр:
    if (!mUi->parameterTableView->selectionModel()->selectedRows().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Remove Parameters?"),
                tr("Are you sure you want to remove the selected parameter records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedParameterIndex,
                     mUi->parameterTableView->selectionModel()->selectedRows()) {
                Id parameterId = mParameterModel->index(selectedParameterIndex.row(), 0)
                        .data().toULongLong();
                DB::deletePhaseParameter(mSelectedPhaseId, parameterId);
            }
            mParameterModel->select();
        }
    } else {
        QMessageBox::information(this, tr("Remove Parameters"),
                                 tr("Select at least one parameter record."));
    }
}
//------------------------------------------------------------------------------
void MainWindow::parameterAddingDialogFinished(int)
{
    mParameterAddingDialog->deleteLater();
    mParameterAddingDialog = 0;
}
//------------------------------------------------------------------------------
void MainWindow::on_createRangePushButton_clicked()
{
    if (mRangeCreationDialog == 0) {
        mRangeCreationDialog = new RangeCreationDialog(mRangeModel, this);
        connect(mRangeCreationDialog, SIGNAL(finished(int)),
                SLOT(rangeCreationDialogFinished(int)));
        mRangeCreationDialog->show();
    } else {
        mRangeCreationDialog->activateWindow();
    }
}
//------------------------------------------------------------------------------
void MainWindow::on_deleteRangePushButton_clicked()
{
    // Если выделен хотя бы один диапазон:
    if (!mUi->rangeTableView->selectionModel()->selectedIndexes().isEmpty()) {
        qint32 clickedButton = QMessageBox::question(this, tr("Delete Ranges?"),
                tr("Are you sure you want to delete the selected range records?"),
                QMessageBox::Ok, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Ok) {
            foreach (QModelIndex selectedIndex,
                     mUi->rangeTableView->selectionModel()->selectedIndexes()) {
                mRangeModel->removeRow(selectedIndex.row());
            }
            mRangeModel->submitAll();
        }
    }
    else {
        QMessageBox::information(this, tr("Delete Ranges"),
                                 tr("Select at least one range record."));
    }
}
//------------------------------------------------------------------------------
void MainWindow::rangeCreationDialogFinished(int)
{
    mRangeCreationDialog->deleteLater();
    mRangeCreationDialog = 0;
}
//------------------------------------------------------------------------------
void MainWindow::rangeChanged(QModelIndex, QModelIndex)
{
    mRangeModel->submitAll();
}
//------------------------------------------------------------------------------

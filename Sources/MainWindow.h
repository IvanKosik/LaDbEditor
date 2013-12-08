#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DB.h"
#include "Dialogs/DiagnosisCreationDialog.h"
#include "Dialogs/PhaseCreationDialog.h"
#include "Dialogs/ParameterAddingDialog.h"
#include "Dialogs/RangeCreationDialog.h"

#include <QMainWindow>

#include <QSqlTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}
//------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool hasSelectedDiagnosis() const;
    Id selectedDiagnosisId() const;

    bool hasSelectedPhase() const;
    Id selectedPhaseId() const;

    bool hasSelectedParameter() const;
    Id selectedParameterId() const;

    void setWasPhasesChanged(bool wasPhasesChanged);
    bool wasPhasesChanged() const;
    
    void savePhaseChanges();

private:
    void createModels();
    void createDiagnosisModel();
    void createPhaseModel();
    void createParameterModel();
    void createRangeModel();


    Ui::MainWindow *mUi;

    QSqlTableModel *mPhaseModel;
    QSqlTableModel *mDiagnosisModel;
    QSqlTableModel *mParameterModel;
    QSqlTableModel *mRangeModel;

    QSortFilterProxyModel *mDiagnosisProxyModel; //% Если используется только в одном месте (конструкторе), то убрать ее из полей класса.
    QSortFilterProxyModel *mPhaseProxyModel;
    QSortFilterProxyModel *mParameterProxyModel;
    QSortFilterProxyModel *mRangeProxyModel;

    bool mHasSelectedDiagnosis;
    bool mHasSelectedPhase;
    bool mHasSelectedParameter;

    Id mSelectedDiagnosisId;
    Id mSelectedPhaseId;
    Id mSelectedParameterId;

    bool mWasPhasesChanged;

    DiagnosisCreationDialog *mDiagnosisCreationDialog;
    PhaseCreationDialog *mPhaseCreationDialog;
    ParameterAddingDialog *mParameterAddingDialog;
    RangeCreationDialog *mRangeCreationDialog;

private slots:
    void on_createDiagnosisPushButton_clicked();
    void on_deleteDiagnosisPushButton_clicked();
    void diagnosisCreationDialogFinished(int);
    void currentDiagnosisChanged(QModelIndex current, QModelIndex previous);
    void diagnosisChanged(QModelIndex, QModelIndex);

    void on_createPhasePushButton_clicked();
    void on_deletePhasePushButton_clicked();
    void phaseCreationDialogFinished(int);
    void currentPhaseChanged(QModelIndex current, QModelIndex previous);
    void phaseChanged(QModelIndex, QModelIndex);

    void on_addParameterPushButton_clicked();
    void on_removeParameterPushButton_clicked();
    void parameterAddingDialogFinished(int);
    void currentParameterChanged(QModelIndex current, QModelIndex previous);
    void parameterChanged(QModelIndex, QModelIndex);

    void on_createRangePushButton_clicked();
    void on_deleteRangePushButton_clicked();
    void rangeCreationDialogFinished(int);
    void rangeChanged(QModelIndex, QModelIndex);


    /*void on_submitPushButton_clicked();
    void on_revertPushButton_clicked();*/
    void on_quitPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // MAINWINDOW_H

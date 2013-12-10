#ifndef PARAMETERADDINGDIALOG_H
#define PARAMETERADDINGDIALOG_H

#include "DB.h"
#include "ParameterCreationDialog.h"
#include "ResearchAddingDialog.h"

#include <QDialog>

#include <QSqlTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class ParameterAddingDialog;
}
//------------------------------------------------------------------------------
class ParameterAddingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParameterAddingDialog(QWidget *parent = 0);
    ~ParameterAddingDialog();

    bool hasSelectedParameter() const;
    Id selectedParameterId() const;
    
private:
    bool addSelectedParameters();

    Ui::ParameterAddingDialog *mUi;
    QSqlTableModel *mPhaseParameterModel;

    QSqlTableModel *mParameterModel;
    QSqlTableModel *mResearchModel;

    QSortFilterProxyModel *mParameterProxyModel;
    QSortFilterProxyModel *mResearchProxyModel;

    bool mHasSelectedParameter;
    Id mSelectedParameterId;

    ParameterCreationDialog *mParameterCreationDialog;
    ResearchAddingDialog *mResearchAddingDialog;

private slots:
    void on_createParameterPushButton_clicked();
    void on_deleteParameterPushButton_clicked();
    void parameterCreationDialogFinished(int);
    void currentParameterChanged(QModelIndex current, QModelIndex);
    void parameterChanged(QModelIndex, QModelIndex);

    void on_addResearchPushButton_clicked();
    void on_removeResearchPushButton_clicked();
    void researchAddingDialogFinished(int);

    void on_okPushButton_clicked();
    void on_addPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // PARAMETERADDINGDIALOG_H

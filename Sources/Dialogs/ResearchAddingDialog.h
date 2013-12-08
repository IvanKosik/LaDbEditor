#ifndef RESEARCHADDINGDIALOG_H
#define RESEARCHADDINGDIALOG_H

#include "ResearchCreationDialog.h"

#include <QDialog>

#include <QSqlTableModel>
#include <QSortFilterProxyModel>
//------------------------------------------------------------------------------
namespace Ui {
class ResearchAddingDialog;
}

class ResearchAddingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResearchAddingDialog(QSqlTableModel *parameterResearchModel,
                                  QWidget *parent = 0);
    ~ResearchAddingDialog();
        
private:
    bool addSelectedResearches();

    Ui::ResearchAddingDialog *mUi;
    QSqlTableModel *mParameterResearchModel;
    QSqlTableModel *mResearchModel;

    QSortFilterProxyModel *mResearchProxyModel;

    ResearchCreationDialog *mResearchCreationDialog;

private slots:
    void on_createResearchPushButton_clicked();
    void on_deleteResearchPushButton_clicked();
    void researchCreationDialogFinished(int);
    void researchChanged(QModelIndex, QModelIndex);

    void on_okPushButton_clicked();
    void on_addPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // RESEARCHADDINGDIALOG_H

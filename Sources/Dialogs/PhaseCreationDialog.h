#ifndef PHASECREATIONDIALOG_H
#define PHASECREATIONDIALOG_H

#include <QDialog>

#include <QSqlTableModel>

namespace Ui {
    class PhaseCreationDialog;
}
//------------------------------------------------------------------------------
class PhaseCreationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PhaseCreationDialog(QSqlTableModel *phaseModel,
                                 QWidget *parent = 0);
    ~PhaseCreationDialog();
    
private:
    bool createPhase();

    Ui::PhaseCreationDialog *mUi;
    QSqlTableModel *mPhaseModel;

private slots:
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_okPushButton_clicked();
    void on_createPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // PHASECREATIONDIALOG_H

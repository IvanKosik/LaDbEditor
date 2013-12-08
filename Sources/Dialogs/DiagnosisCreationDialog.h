#ifndef DIAGNOSISCREATIONDIALOG_H
#define DIAGNOSISCREATIONDIALOG_H

#include <QDialog>

#include <QSqlTableModel>

namespace Ui {
    class DiagnosisCreationDialog;
}
//------------------------------------------------------------------------------
class DiagnosisCreationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DiagnosisCreationDialog(QSqlTableModel *diagnosisModel,
                                     QWidget *parent = 0);
    ~DiagnosisCreationDialog();
    
private:
    bool createDiagnosis();

    Ui::DiagnosisCreationDialog *mUi;
    QSqlTableModel *mDiagnosisModel;

private slots:
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_okPushButton_clicked();
    void on_createPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // DIAGNOSISCREATIONDIALOG_H

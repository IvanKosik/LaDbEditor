#ifndef PARAMETERCREATIONDIALOG_H
#define PARAMETERCREATIONDIALOG_H

#include <QDialog>

#include <QSqlTableModel>

namespace Ui {
    class ParameterCreationDialog;
}
//------------------------------------------------------------------------------
class ParameterCreationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParameterCreationDialog(QSqlTableModel *parameterModel,
                                     QWidget *parent = 0);
    ~ParameterCreationDialog();
    
private:
    bool createParameter();

    Ui::ParameterCreationDialog *mUi;
    QSqlTableModel *mParameterModel;

private slots:
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_okPushButton_clicked();
    void on_createPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // PARAMETERCREATIONDIALOG_H

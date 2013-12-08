#ifndef RANGECREATIONDIALOG_H
#define RANGECREATIONDIALOG_H

#include <QDialog>

#include <QSqlTableModel>

namespace Ui {
    class RangeCreationDialog;
}
//------------------------------------------------------------------------------
class RangeCreationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RangeCreationDialog(QSqlTableModel *rangeModel,
                                 QWidget *parent = 0);
    ~RangeCreationDialog();
    
private:
    bool createRange();
    void disableAcceptButtons(bool disable);

    Ui::RangeCreationDialog *mUi;
    QSqlTableModel *mRangeModel;

private slots:
    void on_rangeBeginLineEdit_textChanged(const QString &arg1);
    void on_rangeEndLineEdit_textChanged(const QString &arg1);
    void on_beginProbabilityLineEdit_textChanged(const QString &arg1);
    void on_endProbabilityLineEdit_textChanged(const QString &arg1);

    void on_okPushButton_clicked();
    void on_createPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // RANGECREATIONDIALOG_H

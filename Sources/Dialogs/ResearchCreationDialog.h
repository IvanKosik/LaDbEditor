#ifndef RESEARCHCREATIONDIALOG_H
#define RESEARCHCREATIONDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
    class ResearchCreationDialog;
}
//------------------------------------------------------------------------------
class ResearchCreationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResearchCreationDialog(QSqlTableModel *researchModel,
                                    QWidget *parent = 0);
    ~ResearchCreationDialog();
    
private:
    bool createResearch();

    Ui::ResearchCreationDialog *mUi;
    QSqlTableModel *mResearchModel;

private slots:
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_okPushButton_clicked();
    void on_createPushButton_clicked();
    void on_cancelPushButton_clicked();
};
//------------------------------------------------------------------------------
#endif // RESEARCHCREATIONDIALOG_H

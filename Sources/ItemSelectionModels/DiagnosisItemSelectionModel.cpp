#include "DiagnosisItemSelectionModel.h"

#include <QMessageBox>

#include <QDebug> //%
//------------------------------------------------------------------------------
DiagnosisItemSelectionModel::DiagnosisItemSelectionModel(QAbstractItemModel *model,
                                                         QObject *parent)
    : ItemSelectionModel(model, parent)
{
}
//------------------------------------------------------------------------------
void DiagnosisItemSelectionModel::currentRowChangedSlot(QModelIndex current,
                                                        QModelIndex previous)
{
//    static bool wasCancelClicked;

//    // Если выделен хотя бы один ряд:
//    if (current.isValid()) {
//        // Если фазы были изменены и Cancel не был нажат на вопрос о сохранении:
//        if (mMainWindow->wasPhasesChanged() && !wasCancelClicked) {
//            // Спросим, желает ли пользователь сохранить изменения:
//            qint32 clickedButton = QMessageBox::question(this, tr("Save Changes?"),
//                    tr("Do you want to save the phase changes?"),
//                    QMessageBox::Save, QMessageBox::No, QMessageBox::Cancel);
//            switch (clickedButton) {
//            case QMessageBox::Save:
//                mMainWindow->savePhaseChanges();
//                wasCancelClicked = false;
//                break;
//            case QMessageBox::No:
//                wasCancelClicked = false;
//                break;
//            case QMessageBox::Cancel:
//                wasCancelClicked = true;
//                mUi->diagnosisTableView->selectRow(previous.row());
//                return;
//                break;
//            }
//        }
//        mHasSelectedDiagnosis = true;
//        // Получаем идентификатор текущего выделенного диагноза:
//        mSelectedDiagnosisId = mDiagnosisModel->index(current.row(), 0).data().toULongLong();
//        // Отображаем только те фазы, которые принадлежат выделенному диагнозу:
//        mPhaseModel->setFilter("DiagnosisID=" + QString::number(mSelectedDiagnosisId));
//        mWasPhasesChanged = false;
//    } else {
//        mHasSelectedDiagnosis = false;
//        mPhaseModel->clear(); //%Ведь нет выделенного диагноза
//    }
}
//------------------------------------------------------------------------------

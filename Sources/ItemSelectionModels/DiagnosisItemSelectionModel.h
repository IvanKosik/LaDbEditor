#ifndef DIAGNOSISITEMSELECTIONMODEL_H
#define DIAGNOSISITEMSELECTIONMODEL_H

#include "ItemSelectionModel.h"
//------------------------------------------------------------------------------
class DiagnosisItemSelectionModel : public ItemSelectionModel
{
    Q_OBJECT
public:
    explicit DiagnosisItemSelectionModel(QAbstractItemModel *model, QObject *parent = 0);
    
private slots:
    void currentRowChangedSlot(QModelIndex current, QModelIndex previous);
};
//------------------------------------------------------------------------------
#endif // DIAGNOSISITEMSELECTIONMODEL_H

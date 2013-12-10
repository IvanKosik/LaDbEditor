#ifndef PHASEPARAMETERMODEL_H
#define PHASEPARAMETERMODEL_H

#include "DB.h"

#include <QSqlQueryModel>
//-----------------------------------------------------------------------------
class PhaseParameterModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    PhaseParameterModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void update(Id phaseId);
    void update();
private:
    bool setWeight(Id phaseParameterId, double weight);

    Id mPhaseId;
};
//-----------------------------------------------------------------------------
#endif // PHASEPARAMETERMODEL_H

#include "PhaseParameterModel.h"

#include "SqlQuery.h"

#include <QtSql>
//-----------------------------------------------------------------------------
PhaseParameterModel::PhaseParameterModel(QObject *parent)
    : QSqlQueryModel(parent), mPhaseId(0)
{
}
//-----------------------------------------------------------------------------
Qt::ItemFlags PhaseParameterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 2) { // Weight column is editable.
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}
//-----------------------------------------------------------------------------
bool PhaseParameterModel::setData(const QModelIndex &index
                                  , const QVariant &value, int role)
{
    if (index.column() != 2) {
        return false;
    }

    QModelIndex phaseParameterIdIndex = QSqlQueryModel::index(index.row(), 0);
    Id phaseParameterId = data(phaseParameterIdIndex).toULongLong();

    clear(); //%?

    bool result = setWeight(phaseParameterId, value.toDouble());
    update();
    return result;
}
//-----------------------------------------------------------------------------
bool PhaseParameterModel::setWeight(Id phaseParameterId, double weight)
{
    SqlQuery updateQuery;
    updateQuery.prepare("UPDATE PhaseParameter SET Weight = :weight WHERE ID = :id");
    updateQuery.bindValue(":weight", weight);
    updateQuery.bindValue(":id", phaseParameterId);
    return updateQuery.exec();
}
//-----------------------------------------------------------------------------
void PhaseParameterModel::update(Id phaseId)
{
    mPhaseId = phaseId;
    update();
}
//-----------------------------------------------------------------------------
void PhaseParameterModel::update()
{
    SqlQuery parameterWeightQuery;
    parameterWeightQuery.prepare(QString("SELECT %4.%1, %2, %3 FROM %4 INNER JOIN %5 ON %4.%6 = %5.%7 WHERE %8 = :phaseId ORDER BY %3 DESC")
                                 .arg("ID", "Name", "Weight", "PhaseParameter"
                                      , "Parameter", "ParameterID", "ID"
                                      , "PhaseID"));  //%! Тут нужно все-таки %4.%1 т.е. Id другой.
    parameterWeightQuery.addBindValue(mPhaseId);
    parameterWeightQuery.exec();
    setQuery(parameterWeightQuery);

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Weight"));
}
//-----------------------------------------------------------------------------

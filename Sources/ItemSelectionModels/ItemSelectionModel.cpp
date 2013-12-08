#include "ItemSelectionModel.h"

#include <QDebug> //%
//------------------------------------------------------------------------------
ItemSelectionModel::ItemSelectionModel(QAbstractItemModel *model, QObject *parent)
    : QItemSelectionModel(model, parent)
{
    mMainWindow = (MainWindow *)parent;

    connect(this, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(currentRowChangedSlot(QModelIndex, QModelIndex)));
}
//------------------------------------------------------------------------------
void ItemSelectionModel::currentRowChangedSlot(QModelIndex current,
                                               QModelIndex previous)
{
}
//------------------------------------------------------------------------------

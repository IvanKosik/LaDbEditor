#ifndef ITEMSELECTIONMODEL_H
#define ITEMSELECTIONMODEL_H

#include "MainWindow.h"

#include <QItemSelectionModel>
//------------------------------------------------------------------------------
class ItemSelectionModel : public QItemSelectionModel
{
    Q_OBJECT

public:
    explicit ItemSelectionModel(QAbstractItemModel *model, QObject *parent = 0);

protected:
    MainWindow *mMainWindow;

protected slots:
    virtual void currentRowChangedSlot(QModelIndex current, QModelIndex previous);
};
//------------------------------------------------------------------------------
#endif // ITEMSELECTIONMODEL_H

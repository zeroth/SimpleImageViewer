#include "LayerModel.h"


LayerModel::LayerModel(QObject *parent) : QAbstractListModel(parent)
{

}

int LayerModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant LayerModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}


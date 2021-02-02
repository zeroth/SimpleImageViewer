#ifndef LAYERMODEL_H
#define LAYERMODEL_H

#include <QObject>
#include <QAbstractListModel>

class LayerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    LayerModel(const LayerModel&) = delete;
    LayerModel& operator=(const LayerModel &) = delete;
    LayerModel(LayerModel &&) = delete;
    LayerModel & operator=(LayerModel &&) = delete;

    static LayerModel& instance(){
        static LayerModel test;
        return test;
    }

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    LayerModel(QObject *parent = nullptr);


};

#endif // LAYERMODEL_H

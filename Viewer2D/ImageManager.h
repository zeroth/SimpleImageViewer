#pragma once

#include <QObject>
#include "BioImage.h"
#include <QAbstractListModel>
#include <QFileInfoList>
#include "ColorTable.h"
#include <QUrl>

class ImageManager : public QAbstractListModel
{
    Q_OBJECT
public:

    static ImageManager& instance(){
        static ImageManager test;
        return test;
    }

    enum Roles
    {
        BioImgName = Qt::UserRole,
        BioImgPath,
        BioImgIsTimeSeries
    };

    Q_INVOKABLE int addImage(const QString& val);
    Q_INVOKABLE int addImage(const QUrl& val);
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    zeroth::BioImage* imageAt(int row);
    Q_INVOKABLE QString imagePathAt(int row) const;

    Q_INVOKABLE int width(int row) {
        return this->imageAt(row)->width();
    }
    Q_INVOKABLE int height(int row) {
        return this->imageAt(row)->height();
    }
    Q_INVOKABLE int length(int row) {
        return this->imageAt(row)->length();
    }
    Q_INVOKABLE double min(int row) {
        return this->imageAt(row)->min();
    }
    Q_INVOKABLE double max(int row) {
        return this->imageAt(row)->max();
    }
    Q_INVOKABLE double intensityAt(int row, int x, int y, int z) {
        return this->imageAt(row)->intensityAt(x, y, z);
    }
    // Helper function
    Q_INVOKABLE QString toLocalPath(const QUrl& path) {
        return path.toLocalFile();
    }

    ///////////////////////////////////////////////////////////
    // Singleton precautions
    ImageManager(const ImageManager&) = delete;
    ImageManager& operator=(const ImageManager &) = delete;
    ImageManager(ImageManager &&) = delete;
    ImageManager & operator=(ImageManager &&) = delete;
    Q_INVOKABLE QStringList availableColorMaps();
    QVector<QRgb> colorTable(const QString &name);

    Q_INVOKABLE void exeAnisotropy(const QString &parallelBackGround, const QString &perpendicularBackground, const QString &parallel, const QString &perpendicular, double subtractVal);
private: // make it singleton
    ImageManager(QObject *parent = nullptr);
    ///////////////////////////////////////////////////////////

private:
    QFileInfoList m_layers;
    ColorTable* cTable;
    QHash<int, zeroth::BioImage*> imageCache;
};


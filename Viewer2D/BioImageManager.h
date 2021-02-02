#ifndef BIOIMAGEMANAGER_H
#define BIOIMAGEMANAGER_H

#include <QObject>
#include "BioImage.h"
#include <QQmlEngine>
#include <QJSEngine>
#include <QHash>
#include "ColorTable.h"


class BioImageManager : public QObject
{
    Q_OBJECT
public:
    BioImageManager(const BioImageManager&) = delete;
    BioImageManager& operator=(const BioImageManager &) = delete;
    BioImageManager(BioImageManager &&) = delete;
    BioImageManager & operator=(BioImageManager &&) = delete;

    static BioImageManager& instance(){
        static BioImageManager test;
        return test;
    }

    Q_INVOKABLE void createImage(const QString &path);

    Q_INVOKABLE void saveImage(const QString& id, const QString& path);
    Q_INVOKABLE void setImage(const QString& id, zeroth::BioImage* img);
    void setImage(const QString& id, const QString& path);
    zeroth::BioImage* image(const QString& id);
    Q_INVOKABLE QString addImage(const QUrl &path);

    Q_INVOKABLE float intensity(const QString& id, qreal x, qreal y, qreal z);
    Q_INVOKABLE double imgMin(const QString& id);
    Q_INVOKABLE double imgMax(const QString& id);
    Q_INVOKABLE int imgWidth(const QString& id);
    Q_INVOKABLE int imgHeight(const QString& id);
    Q_INVOKABLE int imgLength(const QString& id);
    Q_INVOKABLE QStringList availableColorMaps();
    QVector<QRgb> colorTable(const QString& name);
    Q_INVOKABLE bool hasImage();

    Q_INVOKABLE QStringList imageIdList() const {
        return m_images.keys();
    }

    QHash<QString, zeroth::BioImage*>& imageTable() {
        return m_images;
    }
    Q_INVOKABLE QString toLocalPath(const QUrl& path) {
        return path.toLocalFile();
    }

signals:
    void imageLoaded(const QString& id);
    void imageUpdated();
//    void anisotropyReady(const QString& rId, const QString& tId);
    void dataModelUpdated();



private:
    BioImageManager(QObject *parent = nullptr);

private:
    QHash<QString, zeroth::BioImage*> m_images;
    ColorTable* cTable;
};

#endif // BIOIMAGEMANAGER_H

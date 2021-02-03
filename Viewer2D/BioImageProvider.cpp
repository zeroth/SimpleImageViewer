#include "BioImageProvider.h"
#include "BioImage.h"
//#include "BioImageManager.h"
#include "ImageManager.h"
#include <QDebug>
#include <QString>

BioImageProvider::BioImageProvider()
    :QQuickImageProvider(QQuickImageProvider::Image)
{
    this->name = "Something";
}

QImage BioImageProvider::requestImage(const QString &id, QSize *size, const QSize & requestedSize)
{

    // Bi/{imageid}/{time_point}/{channel}/{page_number_z}/{threshold_min}/{threshold_min}/{colormapname}
    // ""/  0     /     1      /    2    /      3        /      4        /      5        /      6
    int imageId = id.section('/', 0, 0).toInt();
    int timePoint = id.section('/', 1, 1).toInt();
    Q_UNUSED(timePoint);

    QString channel = id.section('/', 2, 2);
    int z = id.section('/', 3, 3).toInt();
    QString thresholdMinStr = id.section('/', 4, 4);
    QString thresholdMaxStr = id.section('/', 5, 5);

    QString colorMap = id.section('/', 6, 6);

    QVector<QRgb> qtColorTable = ImageManager::instance().colorTable(colorMap);
    zeroth::BioImage* img = ImageManager::instance().imageAt(imageId);

    if(!img) {
        qDebug() << "No Image at this index";
        return QImage();
    }

    float thresholdMin = thresholdMinStr == "nan" ? img->min() : thresholdMinStr.toFloat();
    float thresholdMax = thresholdMaxStr == "nan" ? img->max() : thresholdMaxStr.toFloat();

    QImage qImg(img->width(), img->height(), QImage::Format_Indexed8);

    img->displayImage(&qImg, z, thresholdMin, thresholdMax);

    qImg.setColorTable(qtColorTable);

    if(size)
            *size = qImg.size();

    if(requestedSize.isValid()){
        qDebug() << "requestedSize " << requestedSize;
        qImg = qImg.scaled(requestedSize,
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
    }

    return qImg;

}

/*QImage BioImageProvider::requestImage(const QString &id, QSize *size, const QSize &)
{
    // find the parametors

    // Bi/img/{imageid}/cmap/{colormapname}/min/{min}/max/{max}/t/{on/off}/s/{page_numper}
    QStringList idSplit = id.split('/');
    QString imageId = idSplit.contains("img") ? idSplit.at(idSplit.indexOf("img")+1) : "";

    if(imageId == "") {
        return QImage();
    }
    zeroth::BioImage* img = BioImageManager::instance().image(imageId);
    if(!img) {
        return QImage();
    }
    // TODO: Improve the BioImage loading to load only require page
    QString colorMap = idSplit.contains("cmap") ?idSplit.at(idSplit.indexOf("cmap")+1): "Grays";
    double min = idSplit.contains("min") ? idSplit.at(idSplit.indexOf("min")+1).toDouble(): BioImageManager::instance().imgMin(imageId);
    double max = idSplit.contains("max") ? idSplit.at(idSplit.indexOf("max")+1).toDouble(): BioImageManager::instance().imgMax(imageId);
    bool threshold = idSplit.contains("t") ?idSplit.at(idSplit.indexOf("t")+1).toInt(): 0;
    int pageIndex = idSplit.contains("s") ?idSplit.at(idSplit.indexOf("s")+1).toInt(): 0;
    qDebug() << "page index " << pageIndex;
    qDebug() << id;
    qDebug() << colorMap << min <<  max << threshold;
    qDebug() << img->width()<< " X " <<img->height();

    QVector<QRgb> qtColorTable = BioImageManager::instance().colorTable(colorMap);

    QImage qImg(img->width(), img->height(), QImage::Format_Indexed8);

    img->displayImage(&qImg, pageIndex, min, max);

    qImg.setColorTable(qtColorTable);
    size->setWidth(img->width());
    size->setHeight(img->height());
    qImg.save("D:\\test_bioimg.png");

    return qImg;
}*/


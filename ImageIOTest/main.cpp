#include "BioImage.h"
#include <QImage>
#include <QRgb>
#include <QGuiApplication>
#include <QDebug>
#include <type_traits>
#include <limits>
#include <QColorSpace>

// Simple test for BioImage
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    zeroth::BioImage img;
    img.setSource("D:/Lab/SimpleImageViewer/ImageIOTest/data/data_3d.tif");
    qDebug() << qPrintable(img.info());
    qDebug() << "---------------";

    int nStep = 256;
    QVector<QRgb> gColorTable (nStep);
    for (int i = 0; i<nStep; ++i) {

        gColorTable[i] = qRgb(i, i, i);
    }
    QImage qimg;
    img.displayImage(&qimg, 50, 64, 255);
    qimg.setColorTable(gColorTable);
    qimg.save("D:/Lab/SimpleImageViewer/ImageIOTest/data/data_3d_51.png");

    img.save(QString("D:/Lab/SimpleImageViewer/ImageIOTest/data/data_3d_copy_2.tif"));

    app.quit();
    return 0;
}

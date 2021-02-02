#ifndef BIOIMAGEVIEWER_H
#define BIOIMAGEVIEWER_H
#include <QQuickImageProvider>
#include "BioImage.h"

class BioImageProvider : public QQuickImageProvider
{
public:
    BioImageProvider();

    // QQuickImageProvider interface
public:
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QString name;
};

#endif // BIOIMAGEVIEWER_H

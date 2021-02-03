#pragma once

#include <QObject>

struct ImageObj;
struct ImageManagerData;

class ImageManager : public QObject
{
    Q_OBJECT
public:
    ImageManager(QObject *parent = nullptr);
    QString& addImage(const QString& path) const;


private:
    ImageManagerData* d;
};


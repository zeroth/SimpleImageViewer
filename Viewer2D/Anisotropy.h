#ifndef ANISOTROPY_H
#define ANISOTROPY_H

#include <QObject>
#include "BioImage.h"

struct AnisotropyData;

class Anisotropy : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString parallelBackGround
               READ parallelBackground
               WRITE setParallelBackground
               NOTIFY parallerlBackgroundChanged)

    Q_PROPERTY(QString perpendicularBackground
               READ perpendicularBackground
               WRITE setPerpendicularBackground
               NOTIFY perpendicularBackgroundChanged)

    Q_PROPERTY(QString parallel READ parallel WRITE setParallel NOTIFY dataChanged)
    Q_PROPERTY(QString perpendicular READ perpendicular WRITE setPerpendicular NOTIFY dataChanged)
    Q_PROPERTY(double subtractVal READ subtractVal WRITE setSubtractVal NOTIFY dataChanged)

public:
    explicit Anisotropy(QObject *parent = nullptr);

    QString parallelBackground() const;
    void  setParallelBackground(const QString &val);

    QString perpendicularBackground() const;
    void  setPerpendicularBackground(const QString &val);

    QString parallel() const;
    void  setParallel(const QString &val);

    QString perpendicular() const;
    void  setPerpendicular(const QString &val);

    double subtractVal() const;
    void  setSubtractVal(double val);

    Q_INVOKABLE void apply();

    zeroth::BioImage* getImageR();
    zeroth::BioImage* getImageT();

signals:
    void parallerlBackgroundChanged();
    void perpendicularBackgroundChanged();
    void dataChanged();

private:
    AnisotropyData *d;

};

#endif // ANISOTROPY_H

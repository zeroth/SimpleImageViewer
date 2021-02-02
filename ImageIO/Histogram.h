#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QObject>

class Histogram : public QObject
{
    Q_OBJECT
public:
    Histogram(QObject *parent = nullptr);
    double begin;
    double end;
    QList<qreal> hist;
    QList<int> bins;
    int range;

signals:

};

#endif // HISTOGRAM_H

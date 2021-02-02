#ifndef BIOIMAGE_H
#define BIOIMAGE_H

#include <QObject>
#include <QString>
#include "Tiff/Common.h"
#include "Histogram.h"
//namespace zeroth {

struct BioImageData;



class BioImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceUpdate)

public:
    BioImage(QObject *parent = nullptr);
    ~BioImage();

    QString source() const;
    void setSource(const QString& val);
    void update();

    uint32_t width() const;
    void setWidth(uint32_t);

    uint32_t height() const;
    void setHeight(uint32_t );

    uint32_t depth() const;
    void setDepth(uint32_t);

    zeroth::DataType dtype() const;
    void setDtype(zeroth::DataType format);
    uint32_t byteSize() const;

    double imageMin() const;
    double imageMax() const;

    void* data();
    void setData(void* data);

    void* pageAt(int index);
    uint32_t pageSize() const;

    uint8_t* toUint8(); // to display
    float* toFloat(); // to calculate

    template<typename T>
    T* data();

    void save(const QString& path);
    void save8Bit(const QString& path);

    Q_INVOKABLE Histogram* histogram();

    uint8_t *displayImage(double thresholdMin = 0, double thresholdMax = 0, bool threshold=false);
    float intensityAt(uint32_t x, uint32_t y, uint32_t z) const;
    float intensityAt(uint32_t index) const;

    /*uint32_t operator()(uint32_t x, uint32_t y, uint32_t z) const;

    // https://en.wikipedia.org/wiki/Operator_overloading
    BioImage& operator+(const BioImage& other);
    BioImage& operator+(double val);

    BioImage& operator-(const BioImage& other);
    BioImage& operator-(double val);

    BioImage& operator*(const BioImage& other);
    BioImage& operator*(double val);

    BioImage& operator/(const BioImage& other);
    BioImage& operator/(double val);*/

signals:
    void widthChanged();
    void heightChanged();
    void depthChanged();
    void sourceUpdate();

private:
    BioImageData *d;
};
//}
#endif // BIOIMAGE_H

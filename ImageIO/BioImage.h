#ifndef BIOIMAGE_H
#define BIOIMAGE_H

#include <QObject>
#include <QString>
#include "Tiff/Common.h"

namespace zeroth {

struct BioImageData;



class BioImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)
    // source can be file path or directory path
    // TODO: update the code to support directory path
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceUpdate)

public:
    BioImage(QObject *parent = nullptr);
    BioImage(void* data, uint32_t width, uint32_t height, uint32_t length=1, zeroth::DataType dType= zeroth::UINT8, QObject *parent = nullptr);
    BioImage(const QString& source, QObject *parent = nullptr);
    ~BioImage();

    QString source() const;
    void setSource(const QString& val);
    
    void update();

    Q_INVOKABLE uint32_t width() const;
    void setWidth(uint32_t);

    Q_INVOKABLE uint32_t height() const;
    void setHeight(uint32_t );

    Q_INVOKABLE uint32_t length() const;
    void setLength(uint32_t);

    zeroth::DataType dtype() const;
    void setDtype(zeroth::DataType format);

    QString info() const;
    
     uint32_t byteSize() const;

    Q_INVOKABLE double min() const;
    Q_INVOKABLE double max() const;

    void *data();
//    void setData(void* data);
//    template<typename T>
//    T *dataAs();

    void *page(int index);

    void save(const QString& path);

//    template<typename T>
//    void saveAs(const QString& path);


    void displayImage(QImage* image, int pageIndex=0, float thresholdMin = 0, float thresholdMax = 0, bool threshold=false);

    Q_INVOKABLE float intensityAt(int x, int y, int z=0) const;
    Q_INVOKABLE float intensityAt(int index) const;
    float operator()(int x, int y, int z=0) const;

    static QJsonObject imageInfo(const QString& path);
    static QString dtypeToString(zeroth::DataType type);

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

    void *dataAs(zeroth::DataType dtype);
signals:
    void widthChanged();
    void heightChanged();
    void lengthChanged();
    void sourceUpdate();

private:
    BioImageData *d;
};
}
#endif // BIOIMAGE_H

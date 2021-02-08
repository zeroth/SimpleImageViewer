#include "BioImage.h"
#include "TiffReader.h"
#include "TiffWriter.h"
#include <limits>
#include <QImage>
#include <QDebug>
#include <QJsonObject>

namespace zeroth {
struct BioImageData {
    QString source;
    uint8_t* buffer;

    uint32_t width=1;
    uint32_t height=1;
    uint32_t length=1;
    DataType dType = UINT8;
    double min=0;
    double max=0;

    uint16_t bitsPerSample = 8;
    int imageSize = 0;
    int pageSize =0;

    zeroth::TiffReader *reader;

    ~BioImageData() {
        delete [] buffer;
        delete reader;
    }

    template<typename T>
    T normalizeTo(double number, double min, double max) {
        /*
            rmin denote the minimum of the range of your measurement
            rmax denote the maximum of the range of your measurement
            tmin denote the minimum of the range of your desired target scaling
            tmax denote the maximum of the range of your desired target scaling
            m ∈ [rmin,rmax] denote your measurement to be scaled

            Then m = ((m− rmin)/ (rmax−rmin))×(tmax−tmin)+tmin
        */
        return (T)(( (number-min)/(max-min) ) * (double)std::numeric_limits<T>::max());
    }

    template<typename T>
    double calculateMin() {
        T smallest = ((T*)buffer)[0];
        T largest = ((T*)buffer)[0];

        qDebug() << "Image size " << imageSize;
        qDebug() << "smallest  init " << smallest;
        for(int i =1; i < imageSize; i++) {
            if(((T*)buffer)[i] < smallest) {
                smallest = ((T*)buffer)[i];
                qDebug() << "new small " << smallest;
            }
            else if (((T*)buffer)[i] > largest) {
                largest = ((T*)buffer)[i];
            }
//            else if(largest < (double)((T)buffer[i])) {
//                largest  = (double)((T)buffer[i]);
//            }
//            qDebug() << ((T*)buffer)[i];
        }
        qDebug() <<"calculateMin : " << smallest << " : "<< largest;
        return smallest;
    }
    template<typename T>
    double getMin() {
//        std::vector<T> values((T*)buffer, (T*)buffer+imageSize);
//        qDebug() << valuse.size();
        return *std::min_element((T*)buffer, ((T*)buffer)+imageSize);
//        return *std::min_element(values.begin(), values.end());
    }

    template<typename T>
    double getMax() {
        return *std::max_element((T*)buffer, ((T*)buffer)+imageSize);
    }


    uint32_t bitSize(DataType type) {
        switch (type) {
        case UINT8:
        case INT8:
            return 8;
        case UINT16:
        case INT16:
            return 16;

        case UINT32:
        case INT32:
        case FLOAT:
            return 32;

        default :
            return 8;

        }
    }


    uint32_t byteSize(DataType type) {
        return bitSize(type)/8;
    }

    QString dTypeStr()  const {
        switch (dType) {
        case UINT8:
            return "UINT8";
        case INT8:
            return "INT8";
        case UINT16:
            return "UINT16";
        case INT16:
            return "INT16";
        case UINT32:
            return "UINT32";
        case INT32:
            return "INT32";
        case FLOAT:
            return "FLOAT";

        default :
            return "NOT Define";

        }
    }

    template <typename T>
    DataType toDtype(){
        if(std::is_same<T, uint8_t>::value) {
            return UINT8;
        }
        else if(std::is_same<T, int8_t>::value) {
            return INT8;
        }
        else if(std::is_same<T, uint16_t>::value) {
            return UINT16;
        }
        else if(std::is_same<T, int16_t>::value) {
            return INT16;
        }
        else if(std::is_same<T, uint32_t>::value) {
            return UINT32;
        }
        else if(std::is_same<T, int32_t>::value) {
            return INT32;
        }
        else if(std::is_same<T, float>::value) {
            return FLOAT;
        }
        else {
            return UINT8;
        }
    }

    template<typename F, typename T>
    T* convertFrom() {
        T *result = new T[imageSize];
        F* buff = (F*)buffer;
        for(int i = 0; i < imageSize; i++) {
            result[i] = normalizeTo<T>((F)buff[i], min, max);
        }
        return result;
    }

    template<typename T>
    T* convert() {
        switch (dType) {
        case UINT8:
            return convertFrom<uint8_t, T>();
        case INT8:
            return convertFrom<int8_t, T>();
        case UINT16:
            return convertFrom<uint16_t, T>();
        case INT16:
            return convertFrom<int16_t, T>();
        case UINT32:
            return convertFrom<uint32_t, T>();
        case INT32:
            return convertFrom<int32_t, T>();
        case FLOAT:
            return convertFrom<float, T>();

        default :
            return convertFrom<uint8_t, T>();

        }
    }

    template<typename T>
    uint8_t *display8Bit( int pageIndex = 0, T thresholdMin = 0, T thresholdMax = 0) {
        uint8_t* display = (uint8_t*)readPage(pageIndex);
        // y = m . x + b
        // m = 255/(max-min)
        // y =  m . x +b
        // therefor if y = 0
        // b = - (m . x )

        float displayMin = (float)normalizeTo<uint8_t>(thresholdMin, min, max);
        float displayMax = (float)normalizeTo<uint8_t>(thresholdMax, min, max);
        float m = 255/(displayMax-displayMin);
        float b = (m * displayMin) * -1.0;

        for(int i = 0; i < (int)pageSize; i++) {
           T current = ((T*)display)[i];
           if( current <= thresholdMin) {
               current = thresholdMin;
           } else if(current >= thresholdMax) {
               current = thresholdMax;
           }

           float currentThresholdedValue = (float) normalizeTo<uint8_t>(current, thresholdMin, thresholdMax);
           float displayIntensity = (m * currentThresholdedValue) + b;
           if(displayIntensity > 255) {
               displayIntensity = 255;
           } else if(displayIntensity < 0) {
               displayIntensity = 0;
           }
           display[i] = (uint8_t) qRound(displayIntensity);
        }

        return display;
    }

    uint8_t *displayImage(int pageIndex = 0, double thresholdMin = 0, double thresholdMax = 0) {
        switch (dType) {
        case UINT8:
            return display8Bit<uint8_t>(pageIndex, thresholdMin, thresholdMax);
        case INT8:
            return display8Bit<int8_t>(pageIndex, thresholdMin, thresholdMax);
        case UINT16:
            return display8Bit<uint16_t>(pageIndex, thresholdMin, thresholdMax);
        case INT16:
            return display8Bit<int16_t>(pageIndex, thresholdMin, thresholdMax);
        case UINT32:
            return display8Bit<uint32_t>(pageIndex, thresholdMin, thresholdMax);
        case INT32:
            return display8Bit<int32_t>(pageIndex, thresholdMin, thresholdMax);
        case FLOAT:
            return display8Bit<float>(pageIndex, thresholdMin, thresholdMax);

        default :
            return display8Bit<uint8_t>(pageIndex, thresholdMin, thresholdMax);

        }
    }

    float intensityAt(int index) {
        if(index > imageSize)
            return 0;
        switch (dType) {
        case UINT8:
            return ((uint8_t*)(buffer))[index];
        case INT8:
            return ((int8_t*)(buffer))[index];
        case UINT16:
            return ((uint16_t*)(buffer))[index];
        case INT16:
            return ((int16_t*)(buffer))[index];
        case UINT32:
            return ((uint32_t*)(buffer))[index];
        case INT32:
            return ((int32_t*)(buffer))[index];
        case FLOAT:
            return ((float*)(buffer))[index];

        default :
            return ((uint8_t*)(buffer))[index];
        }
    }

    float intensityAt(int x, int y, int z) {
        int index = (z * width * height) + (y * width + x);
        return intensityAt(index);
    }
    void readAll() {
        uint32_t pageSizeByte = width*height * byteSize(dType);
        buffer = new uint8_t[pageSizeByte * length];
        for(uint32_t i = 0; i < length; i++) {
            uint32_t startPoint = i* pageSizeByte;
            memcpy(buffer+startPoint, reader->data(), pageSizeByte );
            reader->next();
        }
    }


    void *readPage(uint32_t index) {
        uint32_t pageSizeByte = width*height * byteSize(dType);
        uint8_t *tmp = new uint8_t[pageSizeByte];
        uint32_t startingPoint = index * pageSizeByte;
        memcpy(tmp, buffer+startingPoint, pageSizeByte);

        return tmp;
    }

    void loadFromSource() {
        reader = new zeroth::TiffReader(source.toStdString());
        width = reader->width();
        height = reader->height();
        length = reader->numberofSlices();
        dType = reader->dataType();
        pageSize = width * height;
        imageSize = pageSize * length;
        bitsPerSample = bitSize(dType);
        readAll();
        // set min and mx
        switch (dType) {
        case UINT8:{
            min = getMin<uint8_t>();
            max = getMax<uint8_t>();
            break;
        }
        case INT8:{
            min = getMin<int8_t>();
            max = getMax<int8_t>();
            break;
        }
        case UINT16:{
//            calculateMin<uint16_t>();
            min = getMin<uint16_t>();
            max = getMax<uint16_t>();
            break;
        }
        case INT16:{
            min = getMin<uint16_t>();
            max = getMax<uint16_t>();
            break;
        }
        case UINT32:{
            min = getMin<uint32_t>();
            max = getMax<uint32_t>();
            break;
        }
        case INT32:{
            min = getMin<int32_t>();
            max = getMax<int32_t>();
            break;
        }
        case FLOAT:{
            min = getMin<float>();
            max = getMax<float>();
            break;
        }

        default :{
            min = getMin<uint8_t>();
            max = getMax<uint8_t>();
            break;
        }


        }

    }


};

BioImage::BioImage(QObject *parent):QObject(parent), d(new BioImageData)
{

}

BioImage::BioImage(void* data,uint32_t width, uint32_t height, uint32_t length, DataType dType, QObject *parent):QObject(parent), d(new BioImageData)
{
    d->width = width;
    d->height = height;
    d->length = length;
    d->dType = dType;

    memcpy(d->buffer, data, d->width*d->height*d->length*d->byteSize(d->dType));
}

BioImage::BioImage(const QString &source, QObject *parent):QObject(parent), d(new BioImageData)
{
    d->source = source;
    d->loadFromSource();
}

BioImage::~BioImage()
{
    qDebug() << "image deleted" << d->source;
    delete d;
}

QString BioImage::source() const
{
    return d->source;
}

void BioImage::setSource(const QString &val)
{
    d->source = val;
    d->loadFromSource();
}

void BioImage::update()
{

}

uint32_t BioImage::width() const
{
    return d->width;
}

void BioImage::setWidth(uint32_t val)
{
    d->width = val;
}

uint32_t BioImage::height() const
{
    return d->height;
}

void BioImage::setHeight(uint32_t val)
{
    d->height = val;
}

uint32_t BioImage::length() const
{
    return d->length;
}

void BioImage::setLength(uint32_t val)
{
    d->length = val;
}

DataType BioImage::dtype() const
{
    return d->dType;
}

void BioImage::setDtype(DataType val)
{
    d->dType = val;
}

QString BioImage::info() const
{
    QStringList data;
    data << "Source : " + d->source
         << "Wdith  : " + QString::number((int)d->width)
         << "Height : " + QString::number((int)d->height)
         << "Length : " + QString::number((int)d->length)
         << "Dtype  : " + d->dTypeStr();
    return data.join('\n');
}

uint32_t BioImage::byteSize() const
{
    return d->byteSize(d->dType);
}

double BioImage::min() const
{
    return d->min;
}

double BioImage::max() const
{
    return d->max;
}

void *BioImage::data()
{
    return d->buffer;
}

/*void BioImage::setData(void *data)
{
  // rather take data in the constructor so we dont screw it up :)
}*/


void *BioImage::dataAs(zeroth::DataType dtype)
{

        switch (dtype) {
        case UINT8:
            return d->convert<uint8_t>();
        case INT8:
            return d->convert<int8_t>();
        case UINT16:
            return d->convert<uint16_t>();
        case INT16:
            return d->convert<int16_t>();
        case UINT32:
            return d->convert<uint32_t>();
        case INT32:
            return d->convert<int32_t>();
        case FLOAT:
            return d->convert<float>();

        default :
            return d->convert<uint8_t>();

        }


}

void * BioImage::page(int index)
{
    return d->readPage(index);
}

void BioImage::save(const QString &path)
{
    zeroth::TiffWriter im(path.toStdString(), d->width, d->height, d->dType);
    uint32_t pageSizeBytes = d->pageSize * d->byteSize(d->dType);
    for(uint32_t i = 0; i < d->length; i++) {
        uint32_t startPoint = i* pageSizeBytes;
        uint8_t* tmp = new uint8_t[pageSizeBytes];
        memcpy(tmp, (uint8_t*)d->buffer+startPoint, pageSizeBytes);
        im.write(tmp);
        delete [] tmp;
    }
    im.close();
}

/*template<typename T>
void BioImage::saveAs(const QString &path)
{

}*/

void BioImage::displayImage(QImage *image, int pageIndex, float thresholdMin, float thresholdMax, bool /*threshold*/)
{
    // make sure image is set with proper size and formate
    if ((image->size().width() == (int)d->width && image->size().height() == (int)d->height) && image->format() != QImage::Format_Indexed8)
            image->reinterpretAsFormat(QImage::Format_Indexed8);

    if ((image->size().width() != (int)d->width && image->size().height() != (int)d->height) || image->format() != QImage::Format_Indexed8 || image->isNull())
        *image = QImage(d->width, d->height, QImage::Format_Indexed8);

    uint8_t* tmp = d->displayImage(pageIndex, thresholdMin, thresholdMax);
    for (int y = 0; y < image->height(); ++y) {
        memcpy(image->scanLine(y),tmp +(y* image->width()), image->width());
    }

}

float BioImage::intensityAt(int x, int y, int z) const
{
    return d->intensityAt(x, y, z);
}

float BioImage::intensityAt(int index) const
{
    return d->intensityAt(index);
}


float BioImage::operator()(int x, int y, int z) const
{
    return this->intensityAt(x, y, z);
}

QJsonObject BioImage::imageInfo(const QString &path)
{
    zeroth::TiffReader reader = zeroth::TiffReader(path.toStdString());
    QJsonObject imgInfo;
    imgInfo.insert("width", int(reader.width()));
    imgInfo.insert("height", int(reader.height()));
    imgInfo.insert("length", int(reader.numberofSlices()));
    return imgInfo;
}

QString BioImage::dtypeToString(DataType type)
{
    switch (type) {
    case UINT8:
        return "UINT8";
    case INT8:
        return "INT8";
    case UINT16:
        return "UINT16";
    case INT16:
        return "INT16";
    case UINT32:
        return "UINT32";
    case INT32:
        return "INT32";
    case FLOAT:
        return "FLOAT";

    default :
        return "NOT Define";

    }
}

}

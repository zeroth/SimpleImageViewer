#include "BioImage.h"
#include "TiffReader.h"
#include "TiffWriter.h"
#include "Common.h"
#include <QMap>
#include <QDebug>
#include<cstring>
uint8_t* testToUint8(uint8_t*) {
    return nullptr;
}

struct HistKey {
    double id;
    bool operator<(const HistKey & o) const {
        return  (id < o.id);
    }
};

struct BioImageData {
    QString path;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    zeroth::DataType dType;
    double min;
    double max;
    uint8_t* buffer;
    std::vector<uint8_t> m_buffer;
    uint16_t bitsPerSample = 8;
    uint32_t imageSize = 0;
    uint32_t pageSize =0;
    QMap<zeroth::DataType, uint16_t> sampleMap;
    bool isUpdated = false;

    double normalizeTo(double number, double min, double max) {
        /*
            rmin denote the minimum of the range of your measurement
            rmax denote the maximum of the range of your measurement
            tmin denote the minimum of the range of your desired target scaling
            tmax denote the maximum of the range of your desired target scaling
            m ∈ [rmin,rmax] denote your measurement to be scaled

            Then m = ((m− rmin)/ (rmax−rmin))×(tmax−tmin)+tmin
        */
        return ( (number-min)/(max-min) ) * 255.0;
    }

    BioImageData(){
        createSampleMap();
        isUpdated = false;
    }
    ~BioImageData(){
        delete [] buffer;
    }
    void createSampleMap() {
        sampleMap[zeroth::UINT8] = 8;
        sampleMap[zeroth::UINT16] = 16;
        sampleMap[zeroth::UINT32] = 32;
        sampleMap[zeroth::INT8] = 8;
        sampleMap[zeroth::INT16] = 16;
        sampleMap[zeroth::INT32] = 32;
        sampleMap[zeroth::FLOAT] = 32;
    }
    uint16_t bytesCnt() {
        return sampleMap[dType]/8;
    }

    template<typename T>
    double getMin() {
        return *std::min_element((T*)buffer, (T*)buffer+imageSize);
    }

    template<typename T>
    double getMax() {
        return *std::max_element((T*)buffer, (T*)buffer+imageSize);
    }

    template<typename T>
    T* convert() {
        T *result = new T[imageSize];
        switch (dType) {
        case zeroth::UINT8: {
            uint8_t* buff = (uint8_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        case zeroth::UINT16: {
            uint16_t* buff = (uint16_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        case zeroth::UINT32: {
            uint32_t* buff = (uint32_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        case zeroth::INT8: {
            int8_t* buff = (int8_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        case zeroth::INT16: {
            int16_t* buff = (int16_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        case zeroth::INT32: {
            int32_t* buff = (int32_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        case zeroth::FLOAT: {
            float* buff = (float*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
            break;
        }
        default: {
            uint8_t* buff = (uint8_t*)buffer;
            for(uint32_t i = 0; i < imageSize; i++) {
                result[i] = (T)buff[i];
            }
        }
        }

        return  result;
    }

    template<typename T>
    void copy(T *data) {
        (T*)buffer = new T[imageSize];

        #pragma omp parallel for
        for(int i =0; i < (int)imageSize; i++) {
            ((T*)buffer)[i] = data[i];
        }

    }

    void readBuffer(zeroth::TiffReader& reader) {
        buffer = new uint8_t[pageSize * depth];
//        m_buffer.reserve(width * height * depth* bytesCnt());
        for(uint32_t i = 0; i < depth; i++) {
            uint32_t startPoint = i* pageSize;
            memcpy(buffer+startPoint, reader.data(), pageSize );
            reader.next();
        }
    }

    void update(){
        switch (dType) {
        case zeroth::UINT8:{
            bitsPerSample =8;
            min = getMin<uint8_t>();
            max = getMax<uint8_t>();
            break;
        }
        case zeroth::UINT16:{
            bitsPerSample = 16;
            min = getMin<uint16_t>();
            max = getMax<uint16_t>();
            break;
        }
        case zeroth::UINT32:{
            bitsPerSample = 32;
            min = getMin<uint32_t>();
            max = getMax<uint32_t>();
            break;
        }
        case zeroth::INT8: {
            bitsPerSample =8;
            min = getMin<int8_t>();
            max = getMax<int8_t>();
            break;
        }
        case zeroth::INT16:{
            bitsPerSample =16;
            min = getMin<int16_t>();
            max = getMax<int16_t>();
            break;
        }
        case zeroth::INT32:{
            bitsPerSample =32;
            min = getMin<int32_t>();
            max = getMax<int32_t>();
            break;
        }
        case zeroth::FLOAT: {
            bitsPerSample =32;
            min = getMin<float>();
            max = getMax<float>();
            break;
        }
        default:{
            bitsPerSample =8;
            min = getMin<uint8_t>();
            max = getMax<uint8_t>();
        }
        }
        isUpdated = true;
    }

    void load() {
        // TODO: find the reader and use it
        // for now assuming its tiff
        zeroth::TiffReader reader(path.toStdString());
        width = reader.width();
        height = reader.height();
        depth = reader.numberofSlices();
        dType = reader.dataType();
        imageSize = width * height * depth;
        pageSize = width* height* bytesCnt();
        // read the data
        readBuffer(reader);
        qDebug() << "Get inensity at 0" << getIntensityAt(0) << " buffer 0 " << buffer[0];
        qDebug() << "Image Inof: "
                 << "\n width : " <<width <<"\n height : " << height << "\n depth : "<< depth
                 << "\n dtype : " <<dType;
                    ;
        isUpdated = false;
        reader.close();
    }

    template<typename T>
        uint8_t* toUint8(T thresholdMin = 0, T thresholdMax = 0, bool threshold=false){
            uint8_t* display = new uint8_t[imageSize];
//            qDebug() << "Threshold : " << thresholdMin <<" - " << thresholdMax ;
    //        T min = *std::min_element((T*)buffer, (T*)buffer+imageSize);
    //        T max = *std::max_element((T*)buffer, (T*)buffer+imageSize);

            // the requested thresholdMin & thresholdMax are not in the range of the actual
            // min and max then ignore thresholding

    //        if(min < thresholdMin  || thresholdMin == 0) {
    //            thresholdMin = min;
    //        }
    //        if( max > thresholdMax || thresholdMax == 0) {
    //            thresholdMax = max;
    //        }

            // y = m.x + b
            // m = 255/(max-min)
            // y =  m . x +b
            // therefor if y = 0
            // b = - (m . x )
            float displayMin = normalizeTo(thresholdMin, min, max);
            float displayMax = normalizeTo(thresholdMax, min, max);
            qDebug() << "image " << min << " - " << max;
            qDebug() << "displayy : " << displayMin <<" - " << displayMax ;
            float m = 255/(displayMax-displayMin);
            float b = (m * displayMin) * -1.0;
    //        float b =  displayMin;


                #pragma omp parallel for
                for(int i =0; i < (int)imageSize; i++) {
                    T current = ((T*)buffer)[i];
                    if( current <= thresholdMin) {
                        current = thresholdMin;
                    } else if(current >= thresholdMax) {
                        current = thresholdMax;
                    }
                    double cd = normalizeTo(current, thresholdMin, thresholdMax);
                    // y = m  x +b
                    // b = convert(thresholdMin, min, max)
                    // newMin = convert(thresholdMin, min, max)
                    // newaMax = convert(thresholdMax, min, max)
                    // m = 255/(newMax-newMin)
                    // cd = m * cd +b
                    float displayIntensity = (m * cd) + b;

//                    qDebug() << i << " Display intensity " << displayIntensity;

                    if(displayIntensity > 255) {
                        displayIntensity = 255;
                    } else if(displayIntensity < 0) {
                        displayIntensity = 0;
                    } /*else {
                        displayIntensity = qRound(displayIntensity);
                    }*/
//                    qDebug() << i << " Current " << current;


//                    switch (i) {
//                    case 0:
//                    case 1359:
//                    case 32225:{
//                        qDebug() << i << " Current " << current;
//                        qDebug() << i << " Display intensity " << displayIntensity;
//                        break;
//                    }

//                    }

                    display[i] = (uint8_t) qRound(displayIntensity);


                }

            qDebug() << "Display 0 " << display[0];
            return display;
        }

        uint8_t* createDisplayImage(double thresholdMin = 0, double thresholdMax = 0, bool threshold=false) {
            switch (dType) {
            case zeroth::UINT8 : {
                return toUint8<uint8_t>(thresholdMin,thresholdMax,threshold);
            }
            case zeroth::UINT16: {
                return toUint8<uint16_t>(thresholdMin,thresholdMax,threshold);
            }
            case zeroth::UINT32:{
                return toUint8<uint32_t>(thresholdMin,thresholdMax,threshold);
            }
            case zeroth::INT8: {
                return toUint8<int8_t>(thresholdMin,thresholdMax,threshold);
            }
            case zeroth::INT16:{
                return toUint8<int16_t>(thresholdMin,thresholdMax,threshold);
            }
            case zeroth::INT32: {
                return toUint8<int32_t>(thresholdMin,thresholdMax,threshold);
            }
            case zeroth::FLOAT:{
                //float
                // TODO :: look for 16 bit float (halp float examples)
                return toUint8<float>(thresholdMin,thresholdMax,threshold);
            }
            default: {
                return toUint8<uint8_t>(thresholdMin,thresholdMax,threshold);
            }

            }
        }

        float getIntensityAt(uint32_t index) {
            if(index > imageSize)
                return 0;
            switch (dType) {
            case zeroth::UINT8 : {
                return ((uint8_t*)(buffer))[index];
            }
            case zeroth::UINT16: {
                return ((uint16_t*)(buffer))[index];
            }
            case zeroth::UINT32:{
                return ((uint32_t*)(buffer))[index];
            }
            case zeroth::INT8: {
                return ((int8_t*)(buffer))[index];
            }
            case zeroth::INT16:{
                return ((int16_t*)(buffer))[index];
            }
            case zeroth::INT32: {
                return ((int32_t*)(buffer))[index];
            }
            case zeroth::FLOAT:{
                //float
                // TODO :: look for 16 bit float (halp float examples)
                return ((float*)(buffer))[index];
            }
            default: {
                return ((uint8_t*)(buffer))[index];
            }

            }
        }

        float getIntensityAt(uint32_t x, uint32_t y, uint32_t z) {
            uint32_t index = (z * width * height) + (y * width + x);
            return getIntensityAt(index);
        }
    // Histogram
    template<typename T>
        QMap<qreal, int> hist()
        {
            QMap<qreal, int> h;


                for(uint32_t i = 0; i < imageSize; i++) {
                    T intensity = ((T*)(buffer))[i];


    //                if(intensity > )
                    if(h.contains(intensity)){
                        h[intensity] = h[intensity]+1;
                    }
                    else {
                        h[intensity] = 1;
                    }
                }

    //            qDebug() << h;
            return h;
        }
        Histogram* histogram() {
            Histogram* hg = new Histogram;
            hg->begin = min;
            hg->end = max;

    //        QMap<double, uint32_t> histMap = hist();
            QMap<qreal, int> histMap;
            switch (dType) {
            case zeroth::UINT8 : {
                histMap = hist<uint8_t>();
                break;
            }
            case zeroth::UINT16: {
                histMap = hist<uint16_t>();
                break;
            }
            case zeroth::UINT32:{
                histMap = hist<uint32_t>();
                break;
            }
            case zeroth::INT8: {
                histMap = hist<int8_t>();
                break;
            }
            case zeroth::INT16:{
                histMap = hist<int16_t>();
                break;
            }
            case zeroth::INT32: {
                histMap = hist<int32_t>();
                break;
            }
            case zeroth::FLOAT:{
                //float
                // TODO :: look for 16 bit float (halp float examples)
                histMap = hist<float>();
                break;
            }
            default: {
                histMap = hist<uint8_t>();
            }
            }

            hg->hist = histMap.keys();
            hg->bins = histMap.values();
            hg->range = histMap.size();

            return hg;
        }
};

BioImage::BioImage(QObject *parent) : QObject(parent), d(new BioImageData)
{
}

BioImage::~BioImage()
{
    delete d;
}

QString BioImage::source() const
{
    return d->path;
}

void BioImage::setSource(const QString &val)
{
    d->path = val;
    d->load();
    this->save("D:\\bio_image_test.tif");
}

void BioImage::update()
{
    // if this is getting called it means the with, height, depth, dataType and data are set
    d->update();
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

uint32_t BioImage::depth() const
{
    return d->depth;
}

void BioImage::setDepth(uint32_t val)
{
    d->depth = val;
}

zeroth::DataType BioImage::dtype() const
{
    return d->dType;
}

void BioImage::setDtype(zeroth::DataType format)
{
    d->dType = format;
}

uint32_t BioImage::byteSize() const
{
    return  d->pageSize * d->depth;
}

double BioImage::imageMin() const
{
    if(!d->isUpdated) d->update();
    return d->min;
}

double BioImage::imageMax() const
{
    if(!d->isUpdated) d->update();
    return d->max;
}

void *BioImage::data()
{
    return d->buffer;
}

void BioImage::setData(void *buffer)
{
    // considering the width, height, depth and dType are set before reaching here this makes it really fast but very unsafe.
//    d->buffer = (uint8_t*)buffer; // memcpy is your friend
    d->imageSize = d->width*d->height*d->depth;
    d->pageSize = d->width*d->height* d->bytesCnt();
    d->buffer = new uint8_t[d->imageSize*d->bytesCnt()];
    memcpy(d->buffer, buffer, d->imageSize*d->bytesCnt());
}

void *BioImage::pageAt(int index)
{
    uint32_t startPoint = index* d->pageSize;
    return d->buffer+startPoint;
}

uint32_t BioImage::pageSize() const
{
    return d->pageSize;
}

uint8_t *BioImage::toUint8()
{
    return d->convert<uint8_t>();
}

float *BioImage::toFloat()
{
    return d->convert<float>();
}

void BioImage::save(const QString& path)
{
    zeroth::TiffWriter im(path.toStdString(), d->width, d->height, d->dType);
    for(uint32_t i = 0; i < d->depth; i++) {
        uint32_t startPoint = i* d->pageSize;
        uint8_t* tmp = new uint8_t[d->pageSize];
        memcpy(tmp, (uint8_t*)d->buffer+startPoint, d->pageSize );
        im.write(tmp);
        delete [] tmp;
    }
    im.close();
}

void BioImage::save8Bit(const QString& path)
{
    zeroth::TiffWriter im(path.toStdString(), d->width, d->height, zeroth::DataType::UINT8);
    uint32_t pSize =  d->width * d->height;
    uint8_t * bff = this->toUint8();
    for(uint32_t i = 0; i < d->depth; i++) {
        uint32_t startPoint = i* pSize;
        uint8_t* tmp = new uint8_t[pSize];
        memcpy(tmp, (uint8_t*)bff+startPoint, pSize);
        im.write(tmp);
        delete [] tmp;
    }
    im.close();
}

Histogram *BioImage::histogram()
{
    return d->histogram();
}

uint8_t *BioImage::displayImage(double thresholdMin, double thresholdMax, bool threshold)
{
    return d->createDisplayImage(thresholdMin,thresholdMax , threshold);
}

float BioImage::intensityAt(uint32_t x, uint32_t y, uint32_t z) const
{
    return d->getIntensityAt(x, y, z);
}

float BioImage::intensityAt(uint32_t index) const
{
    return d->getIntensityAt(index);
}


template<typename T>
T *BioImage::data()
{
    return d->convert<T>();
}

#include "TiffWriter.h"
#include "tiffio.h"
#include <iostream>
#include <string>

namespace zeroth {
struct TiffWriterData {
    std::string path;
    uint32_t width;
    uint32_t height;
    bool isBiggTiff;
    zeroth::DataType sampleFormat;
    TIFF* tif;
    uint8_t samplePerPixel = 1; // grayscale 1
    uint16_t bitsPerSample = 8;
    uint16_t sampleFormatCode = SAMPLEFORMAT_UINT;
    std::string description;
    bool append = false;


    char* imageJDescription() {
        return nullptr;
    }

    void setBitsAndFormat() {
        //using sampleFormat
        switch (sampleFormat) {
        case zeroth::UINT8:{
            bitsPerSample = 8;
            sampleFormatCode = SAMPLEFORMAT_UINT;
            break;
        }
        case zeroth::UINT16:{
            bitsPerSample = 16;
            sampleFormatCode = SAMPLEFORMAT_UINT;
            break;
        }
        case zeroth::UINT32:{
            bitsPerSample = 32;
            sampleFormatCode = SAMPLEFORMAT_UINT;
            break;
        }
        case zeroth::INT8: {
            bitsPerSample = 8;
            sampleFormatCode = SAMPLEFORMAT_INT;
            break;
        }
        case zeroth::INT16:{
            bitsPerSample = 16;
            sampleFormatCode = SAMPLEFORMAT_INT;
            break;
        }
        case zeroth::INT32:{
            bitsPerSample = 32;
            sampleFormatCode = SAMPLEFORMAT_INT;
            break;
        }
        case zeroth::FLOAT: {
            bitsPerSample = 32;
            sampleFormatCode = SAMPLEFORMAT_IEEEFP;
            break;
        }
        }
    }
    void close() {
       if(tif) TIFFClose(tif);
    }
    void init(){
        std::string mode = "w";
        if(append)
            mode = "a";

        if(isBiggTiff)
            mode += "8";


        tif = TIFFOpen(path.c_str(), mode.c_str());
        setBitsAndFormat();

        // if not description generate one
    }

    bool writeDir(void* data) {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitsPerSample);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

        if(!description.empty())  TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, description.c_str());

        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

        tmsize_t bufferSize = TIFFStripSize(tif); //TIFFDefaultStripSize(tif, 0);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, bufferSize);

        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif,TIFFTAG_SAMPLEFORMAT,sampleFormatCode);


        if(!TIFFWriteEncodedStrip(tif, 0, data, bufferSize)) {
            std::cout << "writing error closing the file " << path << " : " << bufferSize << std::endl;
            throw std::runtime_error("writing error closing the file " + path + " : " + std::to_string(bufferSize));
            close();
            return false;
        }
        TIFFWriteDirectory(tif);

        return true;
    }
};

TiffWriter::TiffWriter(const std::string &path, uint32_t width, uint32_t height,  const zeroth::DataType& sampleFormat,
                       const std::string& description, bool isBiggTiff, bool append)
    :d(new TiffWriterData)
{
    d->path = path;
    d->width = width;
    d->height = height;
    d->sampleFormat = sampleFormat;
    d->isBiggTiff = isBiggTiff;
    d->description = description;
    d->append = append;
    d->init();
}

TiffWriter::~TiffWriter()
{
    delete  d;
}

void TiffWriter::write(void *data)
{
    d->writeDir(data);
}

void TiffWriter::close()
{
    d->close();

}
}

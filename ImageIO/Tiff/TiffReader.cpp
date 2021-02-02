#include "TiffReader.h"
#include "tiffio.h"
#include <iostream>
/*
- TIFFTAG_SAMPLESPERPIXEL
SamplesPerPixel is usually 1 for bilevel, grayscale, and palette-color images. SamplesPerPixel is usually 3 for RGB images
- TIFFTAG_BITSPERSAMPLE
    can be 8, 16 or 32 in out case
- TIFFTAG_SAMPLEFORMAT
    LibTiff defines these values, and adds two more to cover complex sample formats:

    SAMPLEFORMAT_UINT = 1; uint8/16/32
    SAMPLEFORMAT_INT = 2; int8/16/32
    SAMPLEFORMAT_IEEEFP = 3; float
    SAMPLEFORMAT_VOID = 4; void
*/
namespace zeroth {
struct TiffReaderData {
    std::string path;
    TIFF* tif;
    int nDirs=0;
    int currentDir = 0;

    // parameters to read form the current dir;
    uint32_t width;
    uint32_t height;
    uint32_t compression;
    uint32_t rowsperstrip;
    uint8_t samplePerPixel;
    uint16_t bitsPerSample;
    uint16_t sampleFormatCode;
    char* description = nullptr;
    int sampleCode =0;
    zeroth::DataType currentDataType;
    tdata_t buffer = nullptr;
    TiffReaderData(){}
    ~TiffReaderData(){
        if(buffer)
            _TIFFfree(buffer);
    }

    void close() {
        if(tif) TIFFClose(tif);
    }

    void setCurrentFormat() {
        switch (sampleCode) {
        case 8:{
            currentDataType = zeroth::UINT8;
            break;
        }
        case 10:{
            currentDataType = zeroth::INT8;
            break;
        }
        case 16:{
            currentDataType = zeroth::UINT16;
            break;
        }
        case 18:{
            currentDataType = zeroth::INT16;
            break;
        }
        case 32:{
            currentDataType = zeroth::UINT32;
            break;
        }
        case 34:{
            currentDataType = zeroth::INT32;
            break;
        }
        case 36:{
            currentDataType = zeroth::FLOAT;
            break;
        }

        }
    }

    void init() {
        tif = TIFFOpen(path.c_str(), "r");

        // count total number of dirs
//        if (tif) {  do {  nDirs++; } while (TIFFReadDirectory(tif));}
        nDirs = TIFFNumberOfDirectories(tif);

        // back to the 1st dir;
        TIFFSetDirectory(tif, 0);

        // check the planner
        uint16_t plannerConfig;
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &plannerConfig);
        if(plannerConfig != PLANARCONFIG_CONTIG){
            std::cerr << "This library doesnot support multiplanner images as of now." << std::endl;
            close();
        }

        // assumming all the directories will have same formating
        if(!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
                !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) ||
                !TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample)||
                !TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplePerPixel)){
            std::cerr << "Not a proper TIFF Image" << std::endl;
            throw std::runtime_error("Not a proper TIFF Image");
            close();
        }


        if(samplePerPixel != 1) {
            std::cerr << "This library does not support non grayscale images as of now" << std::endl;
            throw std::runtime_error( "This library does not support non grayscale images as of now");
            close();
        }

        if(!TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleFormatCode)) {
//            std::cout << "cant read sample formate data type will be determined based on bits per sample." << std::endl;
        }


        if(sampleFormatCode== SAMPLEFORMAT_UINT) {
            sampleCode = bitsPerSample; // 8/16/32
        } else if(sampleFormatCode == SAMPLEFORMAT_INT) {
            sampleCode = SAMPLEFORMAT_INT + bitsPerSample; //10/18/34
        } else if(sampleFormatCode == SAMPLEFORMAT_IEEEFP) {
            sampleCode = 36;
        } else if(sampleFormatCode == SAMPLEFORMAT_VOID) {
            sampleCode = bitsPerSample;
        } else {
            sampleCode = bitsPerSample;
        }
        setCurrentFormat();

#if 0
        uint32_t depth;
        if(!TIFFGetField(tif, TIFFTAG_IMAGEDEPTH, &depth)) {
            std::cout <<("cant read depth");
        } else {
            std::cout << "Depth : " << depth;
        }
#endif
        TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &description);
    }

    bool loadNextDir() {
        return TIFFReadDirectory(tif);
    }
    bool loadDir(int index) {
        return TIFFSetDirectory(tif, index);
    }

    void *readDir() {
        /*uint32_t imageSize = width * height * (bitsPerSample/8);*/
        /*
            Future ref to read multistripe data http://www.libtiff.org/libtiff.html

            TIFF* tif = TIFFOpen("myfile.tif", "r");
            if (tif) {
                tdata_t buf;
                tstrip_t strip;

                buf = _TIFFmalloc(TIFFStripSize(tif));
                for (strip = 0; strip < TIFFNumberOfStrips(tif); strip++)
                    TIFFReadEncodedStrip(tif, strip, buf, (tsize_t) -1);
                _TIFFfree(buf);
                TIFFClose(tif);
            }
        */

        if(buffer)
            _TIFFfree(buffer);
        tmsize_t bufferSize = TIFFStripSize(tif);
        buffer = _TIFFmalloc(bufferSize);
        // if its grayscale there will be only 1 stripe
        if(!TIFFReadEncodedStrip(tif, 0, buffer, bufferSize)) {
            std::cerr << "reading error closing the file " << path << bufferSize << std::endl;
            throw std::runtime_error( "Error while reading strip something went wrong "+ std::to_string(bufferSize));
            close();

        }

        return buffer;

    }
};

TiffReader::TiffReader(const std::string& path) : d(new TiffReaderData)
{
    TIFFSetWarningHandler(0); // just disable all tiff warnings
    d->path = path;
    d->init();
}

TiffReader::~TiffReader()
{
    delete d;
}


bool TiffReader::next()
{
    return d->loadNextDir();
}

bool TiffReader::loadPage(int pageIndex)
{
    return d->loadDir(pageIndex);
}

void *TiffReader::data()
{
    return d->readDir();
}

uint32_t TiffReader::width()
{
    return d->width;
}

uint32_t TiffReader::height()
{
    return d->height;
}

zeroth::DataType TiffReader::dataType() const
{
    return d->currentDataType;
}

void TiffReader::close()
{
    d->close();
}

char *TiffReader::description()
{
    return d->description;
}

int TiffReader::numberofSlices()
{
    return d->nDirs;
}

}

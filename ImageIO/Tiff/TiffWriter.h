#ifndef TIFFWRITER_H
#define TIFFWRITER_H
#include <string>
#include "Common.h"

namespace zeroth {

struct TiffWriterData;
class TiffWriter
{

public:
    TiffWriter(const std::string& path, uint32_t width, uint32_t height,  const zeroth::DataType& sampleFormat,
               const std::string& description = std::string(), bool isBiggTiff=false, bool append = false);
    ~TiffWriter();
    void write(void* data);
    void close();

private:
    TiffWriterData* d;
};
}

#endif // TIFFWRITER_H

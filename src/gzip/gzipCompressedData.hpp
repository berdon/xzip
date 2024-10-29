#ifndef __GZIP_COMPRESSED_DATA__
#define __GZIP_COMPRESSED_DATA__

#include <fstream>

class GZipCompressedData
{
protected:
    /**
     * A buffered iterator to walk through the stream.
     */
    std::istreambuf_iterator<char> streamIterator;

public:
    GZipCompressedData(std::istreambuf_iterator<char> ifstreamIterator);
    virtual void dump() = 0;
    virtual bool decompress(std::ofstream &outputStream) = 0;
};

#endif
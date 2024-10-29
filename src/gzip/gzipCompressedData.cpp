#include "gzipCompressedData.hpp"

GZipCompressedData::GZipCompressedData(std::istreambuf_iterator<char> streamIterator) : streamIterator(streamIterator)
{
}
#ifndef __DEFLATE_COMPRESSED_DATA__
#define __DEFLATE_COMPRESSED_DATA__

#include "deflateHeader.hpp"
#include "../gzipCompressedData.hpp"
#include "../../xtd/compression/huffman.hpp"
#include "../../xtd/stream/istreamBufIteratorReader.hpp"

#include <vector>
#include <memory>
#include <map>

using namespace xtd::compression;

class DeflateCompressedData : public GZipCompressedData
{
protected:
    std::shared_ptr<xtd::IStreamBufIteratorReader<char>> bufReaderPtr;

    DeflateHeader readBlockHeader();
    void dumpDynamicHuffmanBlock();
    HuffmanCoding<char> readCodeLengthsCoding(unsigned char codeLengthCount, bool dump = false);
    std::vector<unsigned char> readCodeLengths(int count);

public:
    DeflateCompressedData(std::istreambuf_iterator<char> streamIterator);
    void dump() override;
    bool decompress(std::ofstream &outputStream) override;
};

#endif
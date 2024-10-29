#ifndef __HUFFMAN_RANGE__
#define __HUFFMAN_RANGE__

#include <vector>

using std::vector;

namespace xtd
{
    namespace compression
    {
        struct HuffmanRange
        {
            unsigned int rangeEnd;
            unsigned int bitLength;
        };

        vector<HuffmanRange> createHuffmanRangeTable(vector<unsigned char> &lengthCodes);
    }
}

#endif
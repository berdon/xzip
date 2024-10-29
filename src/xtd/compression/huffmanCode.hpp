#ifndef __HUFFMAN_CODE__
#define __HUFFMAN_CODE__

#include <string>

using std::string;

namespace xtd
{
    namespace compression
    {
        class HuffmanCode
        {
        public:
            unsigned int value;
            unsigned int bitLength;

            string toString() const;
        };
    }
}

#endif
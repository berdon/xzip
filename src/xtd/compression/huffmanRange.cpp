#include "huffmanRange.hpp"

#include <format>
#include <iostream>
#include <vector>

using std::cout, std::endl;
using std::vector;

namespace xtd
{
    namespace compression
    {
        vector<HuffmanRange> createHuffmanRangeTable(vector<unsigned char> &lengthCodes)
        {
            const static unsigned char codeLengthOrder[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
            auto maxBitLength = *std::max_element(lengthCodes.begin(), lengthCodes.end(), [](unsigned char a, unsigned char b)
                                                  { return a < b; });
            auto huffmanRangeTable = vector<HuffmanRange>(sizeof(codeLengthOrder));

            for (auto i = 0; i < lengthCodes.size(); i++)
            {
                const auto lengthCode = lengthCodes[i];
                const auto symbol = codeLengthOrder[i];
                huffmanRangeTable[symbol].rangeEnd = symbol;
                huffmanRangeTable[codeLengthOrder[i]].bitLength = lengthCode;
            }

            std::cout << std::format("\u250F{:\u2501^27}\u2513", "\u252F") << std::endl;
            std::cout << "\u2503 Code Length \u2502  Bit Length \u2503" << std::endl;
            std::cout << std::format("\u2523{:\u2501^27}\u252B", "\u253F") << std::endl;
            for (auto i = 0; i < huffmanRangeTable.size(); i++)
            {
                const auto symbol = huffmanRangeTable[i].rangeEnd;
                const auto bitLength = huffmanRangeTable[i].bitLength;
                if (bitLength != 0)
                {
                    std::cout << std::format("\u2503 {:11d} \u2502 {:11d} \u2503", (unsigned int)symbol, (unsigned int)bitLength) << std::endl;
                }
            }

            std::cout << std::format("\u2517{:\u2501^27}\u251B", "\u2537") << std::endl;

            return huffmanRangeTable;
        }
    }
}
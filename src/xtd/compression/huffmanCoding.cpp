#include "huffmanCoding.hpp"
#include "huffmanRange.hpp"

#include <format>
#include <iostream>
#include <vector>

using std::cout, std::endl;
using std::format;
using std::vector;

namespace xtd
{
    namespace compression
    {
        HuffmanCoding<char> createHuffmanCoding(vector<HuffmanRange> rangeTable, bool dump)
        {
            // Get a vector of bit lengths
            vector<unsigned int> bitLengths(rangeTable.size());
            std::transform(rangeTable.begin(), rangeTable.end(), bitLengths.begin(),
                           [](HuffmanRange huffmanRange)
                           { return huffmanRange.bitLength; });

            // Grab the max bit length
            auto maxBitLength = *std::max_element(bitLengths.begin(), bitLengths.end());

            // Generate a count of each bit length
            vector<unsigned> bitLengthEntries(maxBitLength);
            std::for_each(bitLengths.begin(), bitLengths.end(), [&bitLengthEntries](auto bitLength)
                          { bitLengthEntries[bitLength] += 1; });

            if (dump)
            {
                cout << format("\u250F{:\u2501^27}\u2513", "\u252F") << endl;
                cout << "\u2503  Bit Length \u2502       Count \u2503" << endl;
                cout << format("\u2523{:\u2501^27}\u252B", "\u253F") << endl;
                for (auto bitLength = 0; bitLength < bitLengthEntries.size(); bitLength++)
                {
                    cout << format("\u2503 {:11d} \u2502 {:11d} \u2503", bitLength, bitLengthEntries[bitLength]) << endl;
                }
                cout << format("\u2517{:\u2501^27}\u251B", "\u2537") << endl;
            }

            // Find the numerical value of the smallest code for each code length
            unsigned int code = 0;
            auto nextCodes = vector<unsigned int>(maxBitLength);

            // Need to clear out the count on 0 bit lengths
            bitLengthEntries[0] = 0;
            for (auto bits = 1; bits <= maxBitLength; bits++)
            {
                code = (code + bitLengthEntries[bits - 1]) << 1;
                nextCodes[bits] = code;
            }

            if (dump)
            {
                cout << format("\u250F{:\u2501^27}\u2513", "\u252F") << endl;
                cout << "\u2503  Bit Length \u2502   Next Code \u2503" << endl;
                cout << format("\u2523{:\u2501^27}\u252B", "\u253F") << endl;
                for (auto bits = 1; bits < maxBitLength; bits++)
                {
                    cout << format("\u2503 {:11d} \u2502 {:11d} \u2503", bits, nextCodes[bits]) << endl;
                }
                cout << format("\u2517{:\u2501^27}\u251B", "\u2537") << endl;
            }

            // Assign numerical values to all codes
            auto symbolToCodeMap = std::map<unsigned int, HuffmanCode>();
            for (const auto huffmanRange : rangeTable)
            {
                auto length = huffmanRange.bitLength;
                if (length != 0)
                {
                    symbolToCodeMap[huffmanRange.rangeEnd] = HuffmanCode{nextCodes[length], length};
                    nextCodes[length] += 1;
                }
            }

            auto coding = HuffmanCoding<char>();
            if (dump)
            {
                cout << format("\u250F{:\u2501^27}\u2513", "\u252F") << endl;
                cout << "\u2503 Code Length \u2502        Code \u2503" << endl;
                cout << format("\u2523{:\u2501^27}\u252B", "\u253F") << endl;
            }
            for (const auto entry : symbolToCodeMap)
            {
                if (dump)
                {
                    cout << format("\u2503 {:11d} \u2502 ", entry.first);
                }
                auto bitDisplay = entry.second.toString();
                coding.put(entry.second, entry.first);

                if (dump)
                {
                    for (auto i = 0; i < 11 - entry.second.bitLength; i++)
                    {
                        cout << " ";
                    }
                    cout << bitDisplay << format(" \u2503") << endl;
                }
            }
            if (dump)
            {
                cout << format("\u2517{:\u2501^27}\u251B", "\u2537") << endl;
            }

            return coding;
        }
    }
}
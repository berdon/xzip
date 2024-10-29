#include "deflateCompressedData.hpp"
#include "../../xtd/stream/istreamBufIteratorReader.hpp"
#include "../../xtd/compression/huffman.hpp"

#include <iostream>
#include <format>
#include <numeric>
#include <algorithm>
#include <map>

using namespace xtd::compression;

DeflateCompressedData::DeflateCompressedData(std::istreambuf_iterator<char> streamIterator)
    : bufReaderPtr(std::make_shared<xtd::IStreamBufIteratorReader<char>>(streamIterator)), GZipCompressedData(streamIterator) {}

DeflateHeader DeflateCompressedData::readBlockHeader()
{
    return DeflateHeader{
        bufReaderPtr.get()->readNextBit() == 0x01,
        bufReaderPtr.get()->readNBits<unsigned char>(2, true)};
}

void DeflateCompressedData::dump()
{
    std::cout << "Deflate Header" << std::endl;
    DeflateHeader blockHeader;
    do
    {
        blockHeader = readBlockHeader();
        std::cout << "- Is Final Block: " << (blockHeader.isFinalBlock ? "true" : "false") << std::endl;
        std::cout << "- Block Type:     " << std::format("{:#04x}", (unsigned char)blockHeader.blockType) << std::endl;

        switch (blockHeader.blockType)
        {
        case 0x00:
        {
            std::cout << "Uncompressed data" << std::endl;
            bufReaderPtr.get()->skipPendingBits();
            unsigned short length = *(++streamIterator) | *(++streamIterator) << 8;
            unsigned short nlength = *(++streamIterator) | *(++streamIterator) << 8;
            std::cout << "- Length:  " << std::format("{:#04x}", (unsigned char)length) << std::endl;
            std::cout << "- NLength: " << std::format("{:#04x}", (unsigned char)nlength) << std::endl;
        }
        break;
        case 0x01:
            std::cout << "Compression Type: Fixed Huffman Codes" << std::endl;
            break;
        case 0x02:
            dumpDynamicHuffmanBlock();
            break;
        default:
            throw std::runtime_error("Impossible deflate block type");
        };
    } while (!blockHeader.isFinalBlock);
}

void DeflateCompressedData::dumpDynamicHuffmanBlock()
{
    std::cout << "Compression Type: Dynamic Huffman Codes" << std::endl;
    unsigned short literalCodeCount = bufReaderPtr.get()->readNBits<unsigned short>(5, true) + 257;
    unsigned short distanceCodeCount = bufReaderPtr.get()->readNBits<unsigned short>(5, true) + 1;
    unsigned char codeLengthCount = bufReaderPtr.get()->readNBits<unsigned char>(4, true) + 4;

    std::cout << "- Literal Codes #:      " << std::format("{:#04x}", (unsigned char)literalCodeCount) << std::endl;
    std::cout << "- Distance Code #:      " << std::format("{:#04x}", (unsigned char)distanceCodeCount) << std::endl;
    std::cout << "- Code Length Code #:   " << std::format("{:#04x}", (unsigned char)codeLengthCount) << std::endl;

    auto codeLengthCoding = readCodeLengthsCoding(codeLengthCount, true);
    // auto literalLengthCoding = readLiteralLengthCoding(literalCodeCount, true)
    unsigned int alphabetSize = literalCodeCount + distanceCodeCount;
    unsigned int literalLengthAlphabet[alphabetSize];
    auto literalLengthCoding = HuffmanCoding<unsigned int>();

    const auto iter = codeLengthCoding.iterator(bufReaderPtr, literalCodeCount);
    unsigned int alphabetWriteIndex = 0;
    for (auto codePtr = iter.begin(); codePtr != iter.end() && alphabetWriteIndex < alphabetSize; codePtr++)
    {
        const auto code = *codePtr;
        switch (code)
        {
        case 16:
        {
            const auto repeatCount = bufReaderPtr.get()->readNBits<unsigned char>(2) + 3;
            for (auto i = 0; i < repeatCount; i++)
            {
                literalLengthAlphabet[alphabetWriteIndex++] = code;
            }
        }
        break;
        case 17:
        {
            auto repeatCount = bufReaderPtr.get()->readNBits<unsigned>(3);
            repeatCount += 3;
            for (auto i = 0; i < repeatCount; i++)
            {
                literalLengthAlphabet[alphabetWriteIndex++] = 0;
            }
        }
        break;
        case 18:
        {
            const auto repeatCount = bufReaderPtr.get()->readNBits<unsigned char>(7) + 11;
            for (auto i = 0; i < repeatCount; i++)
            {
                literalLengthAlphabet[alphabetWriteIndex++] = 0;
            }
        }
        break;
        default:
            literalLengthAlphabet[alphabetWriteIndex++] = code;
        }
    }
    for (auto ch : literalLengthAlphabet)
    {
        std::cout << std::hex << ch << std::endl;
    }
}

HuffmanCoding<char> DeflateCompressedData::readCodeLengthsCoding(unsigned char codeLengthCount, bool dump)
{
    auto codeLengths = readCodeLengths(codeLengthCount);
    auto huffmanRangeTable = createHuffmanRangeTable(codeLengths);
    return createHuffmanCoding(huffmanRangeTable, dump);
}

std::vector<unsigned char> DeflateCompressedData::readCodeLengths(int count)
{
    auto lengthCodes = std::vector<unsigned char>();
    for (auto i = 0; i < count; i++)
    {
        auto codeLength = bufReaderPtr.get()->readNBits<unsigned char>(3, true);
        lengthCodes.push_back(codeLength);
    }

    std::cout << std::format("\u250F{:\u2501^13}\u2513", "") << std::endl;
    std::cout << "\u2503 Length Code \u2503" << std::endl;
    std::cout << std::format("\u2523{:\u2501^13}\u252B", "") << std::endl;
    for (auto lengthCode : lengthCodes)
    {
        std::cout << std::format("\u2503 {:11} \u2503", (unsigned char)lengthCode) << std::endl;
    }
    std::cout << std::format("\u2517{:\u2501^13}\u251B", "") << std::endl;

    return lengthCodes;
}

bool DeflateCompressedData::decompress(std::ofstream &outputStream)
{
    return false;
}
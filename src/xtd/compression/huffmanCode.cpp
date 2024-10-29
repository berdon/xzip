#include "huffmanCode.hpp"

#include <memory>
#include <string>

using xtd::compression::HuffmanCode;

std::string HuffmanCode::toString() const
{
    std::string codeStringPtr;
    auto code = this->value;
    for (auto i = 0; i < this->bitLength; i++)
    {
        unsigned char bit = code & 0x01;
        codeStringPtr.insert(0, bit == 1 ? "1" : "0");
        code >>= 1;
    }
    return codeStringPtr;
}
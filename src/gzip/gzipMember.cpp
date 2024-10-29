#include "gzipMember.hpp"
#include "gzipCompressedData.hpp"
#include "deflate/deflateCompressedData.hpp"

#include <ranges>
#include <vector>
#include <iostream>
#include <memory>

GZipMember::GZipMember(std::istreambuf_iterator<char> iter) : iter(iter)
{
    compressionMethod = (unsigned char)(*iter);
    iter++;

    // unsigned char reserved : 3;      00000XXX
    // unsigned char hasComment : 1;    0000X000
    // unsigned char hasName : 1;       000X0000
    // unsigned char hasExtras : 1;     00X00000
    // unsigned char hasCrc : 1;        0X000000
    // unsigned char isText : 1;        X0000000
    auto flagField = (unsigned char)(*iter);
    this->flags.reserved = (flagField >> 5) & 0x07;
    this->flags.hasComment = ((flagField >> 4) & 0x01) == 0x01;
    this->flags.hasName = ((flagField >> 3) & 0x01) == 0x01;
    this->flags.hasExtras = ((flagField >> 2) & 0x01) == 0x01;
    this->flags.hasCrc = ((flagField >> 1) & 0x01) == 0x01;
    this->flags.isText = ((flagField >> 0) & 0x01) == 0x01;
    *iter++;

    modifiedTime = (unsigned char)(*iter);
    *iter++;
    modifiedTime = modifiedTime | ((unsigned char)(*iter) << 8);
    *iter++;
    modifiedTime = modifiedTime | ((unsigned char)(*iter) << 16);
    *iter++;
    modifiedTime = modifiedTime | ((unsigned char)(*iter) << 24);
    *iter++;

    extraFlags = (unsigned char)(*iter);
    *iter++;

    os = (unsigned char)(*iter);
    *iter++;

    if (flags.hasExtras)
    {
        extraFieldLength = (unsigned char)(*iter);
        *iter++;
        extraFieldLength = extraFieldLength | ((unsigned char)(*iter) << 8);
        *iter++;

        auto foo = (*iter);
        std::copy_n(iter, extraFieldLength, extraField.begin());
    }

    if (flags.hasName)
    {
        while (*iter != 0x0)
        {
            filename += (unsigned char)(*iter);
            *iter++;
        }
        *iter++;
    }

    if (flags.hasComment)
    {
        while (*iter != 0x0)
        {
            comment += (unsigned char)(*iter);
            *iter++;
        }
        *iter++;
    }

    if (flags.hasCrc)
    {
        crc = (unsigned char)(*iter);
        *iter++;
        crc = crc | ((unsigned char)(*iter) << 8);
        *iter++;
    }
}

std::shared_ptr<GZipCompressedData> GZipMember::getCompressedData()
{
    if (getCompressionMethod() == 0x08)
    {
        return std::make_shared<DeflateCompressedData>(iter);
    }

    throw std::runtime_error("Unsupported compression method");
}

GZipMemberIterator::GZipMemberIterator(std::string filepath) : filepath(filepath)
{
    file.open(filepath);
    iter = std::istreambuf_iterator<char>(file);
}

std::shared_ptr<GZipMember> GZipMemberIterator::next()
{
    if (iter == std::istreambuf_iterator<char>())
    {
        return std::shared_ptr<GZipMember>();
    }

    bool foundId1;
    for (; iter != std::istreambuf_iterator<char>(); iter++)
    {
        auto byte = (unsigned char)(*iter);
        if (!foundId1)
        {
            foundId1 = byte == 0x1F;

            // Continue to the next byte
            continue;
        }
        if (byte == 0x8B)
        {
            // Move past ID2
            iter++;
            return current = std::make_shared<GZipMember>(iter);
        }
        else
        {
            foundId1 = false;
        }
    }

    return std::shared_ptr<GZipMember>();
}

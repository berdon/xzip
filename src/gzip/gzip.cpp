#include "gzip.hpp"
#include "deflate/deflateCompressedData.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <format>

GZip::GZip() : GZip(GZipConfiguration()) {}

GZip::GZip(GZipConfiguration configuration) {}

void GZip::dump(std::string filepath)
{
    auto file = GZipFile(filepath);
    auto members = file.members();

    while (auto member = members.get()->next().get())
    {
        std::cout << "Compression:    " << std::format("{:#02X}", member->getCompressionMethod()) << std::endl;
        std::cout << "- Is Text:      " << member->getFlags().isText << std::endl;
        std::cout << "- Has CRC:      " << member->getFlags().hasCrc << std::endl;
        std::cout << "- Has Extras:   " << member->getFlags().hasExtras << std::endl;
        std::cout << "- Has Name:     " << member->getFlags().hasName << std::endl;
        std::cout << "- Has Comments: " << member->getFlags().hasComment << std::endl;
        std::cout << "Modified Time:  " << member->getModifiedTime() << std::endl;
        std::cout << "Name:           " << member->getFilename() << std::endl;

        auto compressedMemberPtr = member->getCompressedData();
        auto compressedMember = compressedMemberPtr.get();
        compressedMember->dump();
    }
}

bool GZip::expand(std::string filepath)
{
    return true;
}

bool GZip::compress(std::vector<std::string> filepaths)
{
    return false;
}
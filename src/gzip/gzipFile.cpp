#include "gzipFile.hpp"

#include <memory>

GZipFile::GZipFile(std::string filepath) : filepath(filepath)
{
}

bool GZipFile::open()
{
    if (!std::filesystem::exists(filepath))
    {
        // TODO: Better handling
        throw std::runtime_error("Invalid filepath");
    }

    file.open(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open() || file.eof())
    {
        // TODO: Better handling
        throw std::runtime_error("Invalid file");
    }

    return true;
}

std::shared_ptr<GZipMemberIterator> GZipFile::members()
{
    return std::make_shared<GZipMemberIterator>(filepath);
}
#ifndef __GZIP_MEMBER__
#define __GZIP_MEMBER__

#include "gzipCompressedData.hpp"
#include "deflate/deflateCompressedData.hpp"

#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <string>

const int BUFFER_SIZE = 2048;

struct GZipFlags
{
    unsigned char reserved : 3;
    bool hasComment;
    bool hasName;
    bool hasExtras;
    bool hasCrc;
    bool isText;
};

class GZipMember
{
private:
    std::istreambuf_iterator<char> iter;
    unsigned char compressionMethod;
    GZipFlags flags;
    unsigned int modifiedTime;
    unsigned char extraFlags;
    unsigned char os;
    unsigned short extraFieldLength;
    std::vector<char> extraField;
    std::string filename;
    std::string comment;
    unsigned short crc;

public:
    GZipMember(std::istreambuf_iterator<char> iter);
    unsigned char getCompressionMethod() const { return compressionMethod; }
    GZipFlags getFlags() const { return flags; }
    unsigned int getModifiedTime() const { return modifiedTime; }
    std::string getFilename() const { return filename; }
    std::shared_ptr<GZipCompressedData> getCompressedData();
};

class GZipMemberIterator
{
private:
    std::string filepath;
    std::ifstream file;
    std::istreambuf_iterator<char> iter;
    std::shared_ptr<GZipMember> current;

public:
    GZipMemberIterator(std::string filepath);
    std::shared_ptr<GZipMember> next();
    std::shared_ptr<GZipMember> getCurrent() { return current; }
};

#endif
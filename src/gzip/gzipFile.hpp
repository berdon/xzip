#ifndef __GZIP_FILE__
#define __GZIP_FILE__

#include "gzipMember.hpp"
#include <iostream>
#include <fstream>
#include <string>

class GZipFile
{
private:
    std::string filepath;
    std::ifstream file;

public:
    GZipFile(std::string filepath);
    bool open();
    void close();
    std::shared_ptr<GZipMemberIterator> members();
};

#endif
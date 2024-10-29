#ifndef __GZIP__
#define __GZIP__

#include <string>
#include <vector>

#include "gzipConfiguration.hpp"
#include "gzipFile.hpp"
#include "gzipMember.hpp"

class GZip
{
public:
    GZip();
    GZip(GZipConfiguration configuration);
    void dump(std::string filepath);
    bool expand(std::string filepath);
    bool compress(std::vector<std::string> filepaths);
};

#endif
#ifndef __DEFLATE__
#define __DEFLATE__

struct DeflateHeader
{
    bool isFinalBlock;
    unsigned char blockType;
};

#endif
#ifndef __ISTREAMBUF_ITERATOR_READER__
#define __ISTREAMBUF_ITERATOR_READER__

#include <fstream>
#include <memory>

namespace xtd
{
    /**
     * Wraps an istreambuf_iterator and provides convenience methods for reading
     * bits.
     */
    template <class TType>
    class IStreamBufIteratorReader
    {
    private:
        unsigned char readMask = 0x01;
        std::istreambuf_iterator<TType> iterator;

    public:
        bool operator!=(const IStreamBufIteratorReader &other) const
        {
            return iterator != other.iterator;
        }
        IStreamBufIteratorReader(std::istreambuf_iterator<TType> iterator) : iterator(iterator) {}
        TType readNextBit()
        {
            TType bit = (*iterator & readMask) ? 1 : 0;
            readMask <<= 1;
            if (!readMask)
            {
                readMask = 1;
                iterator++;
                if (iterator == std::istreambuf_iterator<TType>())
                {
                    throw std::runtime_error("Invalid compressed data; not a deflate block");
                }
            }
            return bit;
        }

        void skipPendingBits()
        {
            // Only case we don't proceed to the next byte is if we're at the start of it
            if (readMask != 1)
            {
                readMask = 1;
                iterator++;
                if (iterator == std::istreambuf_iterator<char>())
                {
                    throw std::runtime_error("Invalid compressed data; not a deflate block");
                }
            }
        }

        template <typename TOutType>
        TOutType readNBits(int count, bool invert = true)
        {
            TOutType value = TOutType();
            for (auto i = 0; i < count; i++)
            {
                value |= (readNextBit() << (invert ? i : count - (i + 1)));
            }
            return value;
        }
    };
}

#endif
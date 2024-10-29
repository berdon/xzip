#ifndef __HUFFMAN_CODING__
#define __HUFFMAN_CODING__

#include "huffmanCode.hpp"
#include "huffmanRange.hpp"
#include "../stream/istreamBufIteratorReader.hpp"

#include <memory>
#include <vector>
#include <map>

using std::vector;
using xtd::compression::HuffmanRange;

namespace xtd
{
    namespace compression
    {
        template <class TSymbol>
        class HuffmanCoding
        {
            class Iterator
            {
            private:
                HuffmanCoding<TSymbol> coding;
                std::shared_ptr<xtd::IStreamBufIteratorReader<TSymbol>> readerPtr;
                unsigned int read = 0;
                unsigned int maxToRead;

                std::shared_ptr<TSymbol> current = std::make_shared<TSymbol>();
                std::string buffer;
                unsigned int maxBitLength;
                unsigned int previousCodeLength;

            public:
                Iterator(HuffmanCoding<TSymbol> coding,
                         std::shared_ptr<xtd::IStreamBufIteratorReader<TSymbol>> readerPtr,
                         unsigned int count = 0) : coding(coding), readerPtr(readerPtr), maxToRead(count)
                {
                    maxBitLength = 7;
                }

                Iterator &operator++()
                {
                    advance();
                    return *this;
                }

                Iterator operator++(int)
                {
                    auto old = *this;
                    operator++();
                    return old;
                }

                bool operator!=(const Iterator &other) const
                {
                    return (*readerPtr.get()) != (*other.readerPtr.get()) && read <= other.maxToRead;
                }

                const TSymbol &operator*() const { return *current.get(); }

            private:
                void advance()
                {
                    read++;
                    while (true)
                    {
                        buffer += std::to_string((*readerPtr.get()).readNextBit());
                        // Check if the buffer exists in the map
                        if (coding.contains(buffer))
                        {
                            current = std::make_shared<TSymbol>(coding.get(buffer));
                            buffer.clear();
                            return;
                        }
                        else if (buffer.size() > maxBitLength)
                        {
                            throw std::runtime_error("Invalid or unexpected code.");
                        }
                    }
                }
            };

            class IteratorFactory
            {
            private:
                HuffmanCoding<TSymbol> coding;
                std::shared_ptr<xtd::IStreamBufIteratorReader<TSymbol>> readerPtr;
                unsigned int maxToRead = 0;

            public:
                IteratorFactory(HuffmanCoding<TSymbol> coding,
                                std::shared_ptr<xtd::IStreamBufIteratorReader<TSymbol>> readerPtr,
                                unsigned int count = 0) : coding(coding), readerPtr(readerPtr), maxToRead(count) {}
                Iterator begin() const
                {
                    auto iter = Iterator(coding, readerPtr);
                    auto foo = iter++;
                    return iter;
                }
                Iterator end() const
                {
                    std::istreambuf_iterator<TSymbol> terminalIterator;
                    auto terminalReader = std::make_shared<xtd::IStreamBufIteratorReader<TSymbol>>(terminalIterator);
                    return Iterator(coding, terminalReader, maxToRead);
                }
            };

        private:
            std::map<std::string, TSymbol> mapping;

        public:
            HuffmanCoding() : mapping(std::map<std::string, TSymbol>()) {}

            void put(const HuffmanCode &code, const TSymbol &symbol)
            {
                mapping[code.toString()] = symbol;
            }

            TSymbol get(const HuffmanCode &code) const
            {
                mapping[code.toString()];
            }

            TSymbol get(const std::string &code) const
            {
                return mapping.at(code);
            }

            void remove(const HuffmanCode &code)
            {
                mapping.erase(code.toString());
            }

            bool contains(const HuffmanCode &code)
            {
                return mapping.contains(code.toString());
            }

            bool contains(const std::string &code)
            {
                return mapping.contains(code);
            }

            IteratorFactory iterator(std::shared_ptr<xtd::IStreamBufIteratorReader<TSymbol>> readerPtr) const
            {
                return IteratorFactory(*this, readerPtr);
            }

            IteratorFactory iterator(std::shared_ptr<xtd::IStreamBufIteratorReader<TSymbol>> readerPtr, unsigned int count) const
            {
                return IteratorFactory(*this, readerPtr, count);
            }
        };

        HuffmanCoding<char> createHuffmanCoding(vector<HuffmanRange> rangeTable, bool dump);
    }
}

#endif
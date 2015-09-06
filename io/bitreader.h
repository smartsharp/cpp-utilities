#ifndef IOUTILITIES_BITREADER_H
#define IOUTILITIES_BITREADER_H

#include "c++utilities/conversion/types.h"
#include "c++utilities/application/global.h"

#include <ios>

#include <iostream>

namespace IoUtilities {

class LIB_EXPORT BitReader
{
public:
    BitReader(const char *buffer, std::size_t bufferSize);
    BitReader(const char *buffer, const char *end);

    template<typename intType> intType readBits(byte bitCount);
    byte readBit();
    template<typename intType> intType showBits(byte bitCount);
    void skipBits(std::size_t bitCount);
    void align();
    std::size_t bitsAvailable();
    void reset(const char *buffer, std::size_t bufferSize);
    void reset(const char *buffer, const char *end);

private:
    const byte *m_buffer;
    const byte *m_end;
    byte m_bitsAvail;
};

/*!
 * \brief Constructs a new BitReader.
 * \remarks
 *  - Does not take ownership over the specified \a buffer.
 *  - bufferSize must be equal or greather than 1.
 */
inline BitReader::BitReader(const char *buffer, std::size_t bufferSize) :
    BitReader(buffer, buffer + bufferSize)
{}

/*!
 * \brief Constructs a new BitReader.
 * \remarks
 *  - Does not take ownership over the specified \a buffer.
 *  - \a end must be greather than \a buffer.
 */
inline BitReader::BitReader(const char *buffer, const char *end) :
    m_buffer(reinterpret_cast<const byte *>(buffer)),
    m_end(reinterpret_cast<const byte *>(end)),
    m_bitsAvail(8)
{}

/*!
 * \brief Reads the specified number of bits from the buffer advancing the current position by \a bitCount bits.
 * \param bitCount Specifies the number of bits read.
 * \tparam intType Specifies the type of the returned value.
 * \remarks Does not check whether intType is big enough to hold result.
 * \throws Throws ios_base::failure if the end of the buffer is exceeded.
 *         The reader becomes invalid in that case.
 */
template<typename intType>
intType BitReader::readBits(byte bitCount)
{
    intType val = 0;
    for(byte readAtOnce; bitCount; bitCount -= readAtOnce) {
        if(!m_bitsAvail) {
            if(++m_buffer >= m_end) {
                throw std::ios_base::failure("end of buffer exceeded");
            }
            m_bitsAvail = 8;
        }
        readAtOnce = std::min(bitCount, m_bitsAvail);
        val = (val << readAtOnce) | (((*m_buffer) >> (m_bitsAvail -= readAtOnce)) & (0xFF >> (0x08 - readAtOnce)));
    }
    return val;
}

/*!
 * \brief Reads the one bit from the buffer advancing the current position by one bit.
 * \throws Throws ios_base::failure if the end of the buffer is exceeded.
 *         The reader becomes invalid in that case.
 */
inline byte BitReader::readBit()
{
    return readBits<byte>(1) == 1;
}

/*!
 * \brief Reads the specified number of bits from the buffer without advancing the current position.
 */
template<typename intType>
intType BitReader::showBits(byte bitCount)
{
    auto tmp = this;
    return tmp->readBits<intType>(bitCount);
}

/*!
 * \brief Returns the number of bits which are still available to read.
 */
inline std::size_t BitReader::bitsAvailable()
{
    return ((m_end - m_buffer) * 8) + m_bitsAvail;
}

/*!
 * \brief Resets the reader.
 *  - Does not take ownership over the specified \a buffer.
 *  - bufferSize must be equal or greather than 1.
 */
inline void BitReader::reset(const char *buffer, size_t bufferSize)
{
    m_buffer = reinterpret_cast<const byte *>(buffer);
    m_end = reinterpret_cast<const byte *>(buffer + bufferSize);
    m_bitsAvail = 8;
}

/*!
 * \brief Resets the reader.
 *  - Does not take ownership over the specified \a buffer.
 *  - \a end must be greather than \a buffer.
 */
inline void BitReader::reset(const char *buffer, const char *end)
{
    m_buffer = reinterpret_cast<const byte *>(buffer);
    m_end = reinterpret_cast<const byte *>(end);
    m_bitsAvail = 8;
}

/*!
 * \brief Re-establishes alignment.
 */
inline void BitReader::align()
{
    skipBits(m_bitsAvail);
}

} // namespace IoUtilities

#endif // IOUTILITIES_BITREADER_H

#ifndef CONVERSION_UTILITIES_CONVERT_H
#define CONVERSION_UTILITIES_CONVERT_H

#include "types.h"

#include "c++utilities/application/global.h"

#if defined(__BYTE_ORDER__) && defined(__FLOAT_WORD_ORDER__)
#   if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#       define CONVERSION_UTILITIES_BYTE_ORDER_BIG_ENDIAN
#   elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#       define CONVERSION_UTILITIES_BYTE_ORDER_MIDDLE_ENDIAN
#   elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#       define CONVERSION_UTILITIES_BYTE_ORDER_LITTLE_ENDIAN
#   endif
#   if __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__
#       define CONVERSION_UTILITIES_FLOAT_BYTE_ORDER_BIG_ENDIAN
#   elif __FLOAT_WORD_ORDER__ == __ORDER_PDP_ENDIAN__
#       define CONVERSION_UTILITIES_FLOAT_BYTE_ORDER_MIDDLE_ENDIAN
#   elif __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
#       define CONVERSION_UTILITIES_FLOAT_BYTE_ORDER_LITTLE_ENDIAN
#   endif
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64) || defined(__LITTLE_ENDIAN__) || defined(_little_endian__) || defined(_LITTLE_ENDIAN) || defined(_WIN32_WCE) || defined(WINAPI_FAMILY)
#   define CONVERSION_UTILITIES_BYTE_ORDER_LITTLE_ENDIAN
#   define CONVERSION_UTILITIES_FLOAT_BYTE_ORDER_LITTLE_ENDIAN
#elif defined(__MIPSEB__) || defined(__s390__) || defined(__BIG_ENDIAN__) || defined(_big_endian__) || defined(_BIG_ENDIAN)
#   define CONVERSION_UTILITIES_BYTE_ORDER_BIG_ENDIAN
#   define CONVERSION_UTILITIES_FLOAT_BYTE_ORDER_BIG_ENDIAN
#else
#   error "Unable to determine byte order!"
#endif

#if defined(CONVERSION_UTILITIES_BYTE_ORDER_MIDDLE_ENDIAN)
#   error "Middle endian byte order is not supported!"
#endif
#if defined(CONVERSION_UTILITIES_FLOAT_BYTE_ORDER_MIDDLE_ENDIAN)
#   error "Middle endian byte order is not supported!"
#endif

/*!
 * \namespace ConversionUtilities
 * \brief Contains several functions providing conversions between different data types.
 *
 * binaryconversion.h declares functions which convert base data types to an array of bytes and vice versa.
 * stringconversion.h declares different functions around string conversion such as converting a number to a string and vice versa.
 */
namespace ConversionUtilities
{

/*!
 * \brief Encapsulates functions using the big endian byte order.
 * \sa <a href="http://en.wikipedia.org/wiki/Endianness">Endianness - Wikipedia</a>
 */
namespace BE {

#if defined(CONVERSION_UTILITIES_BYTE_ORDER_LITTLE_ENDIAN)
#   define CONVERSION_UTILITIES_BINARY_CONVERSION_INTERNAL 0
#elif defined(CONVERSION_UTILITIES_BYTE_ORDER_BIG_ENDIAN)
#   define CONVERSION_UTILITIES_BINARY_CONVERSION_INTERNAL 1
#endif
#include "binaryconversionprivate.h"
#undef CONVERSION_UTILITIES_BINARY_CONVERSION_INTERNAL

}

/*!
 * \brief Encapsulates functions using the little endian byte order.
 * \sa <a href="http://en.wikipedia.org/wiki/Endianness">Endianness - Wikipedia</a>
 */
namespace LE {

#if defined(CONVERSION_UTILITIES_BYTE_ORDER_LITTLE_ENDIAN)
#   define CONVERSION_UTILITIES_BINARY_CONVERSION_INTERNAL 1
#elif defined(CONVERSION_UTILITIES_BYTE_ORDER_BIG_ENDIAN)
#   define CONVERSION_UTILITIES_BINARY_CONVERSION_INTERNAL 0
#endif
#include "binaryconversionprivate.h"
#undef CONVERSION_UTILITIES_BINARY_CONVERSION_INTERNAL

}

/*!
 * \brief Returns the 8.8 fixed point representation converted from the specified 32-bit floating point number.
 */
LIB_EXPORT inline uint16 toFixed8(float32 float32value)
{
    return static_cast<uint16>(float32value * 256.0f);
}

/*!
 * \brief Returns a 32-bit floating point number converted from the specified 8.8 fixed point representation.
 */
LIB_EXPORT inline float32 toFloat32(uint16 fixed8value)
{
    return static_cast<float32>(fixed8value) / 256.0f;
}

/*!
 * \brief Returns the 16.16 fixed point representation converted from the specified 32-bit floating point number.
 */
LIB_EXPORT inline uint32 toFixed16(float32 float32value)
{
    return static_cast<uint32>(float32value * 65536.0f);
}

/*!
 * \brief Returns a 32-bit floating point number converted from the specified 16.16 fixed point representation.
 */
LIB_EXPORT inline float32 toFloat32(uint32 fixed16value)
{
    return static_cast<float32>(fixed16value) / 65536.0f;
}

/*!
 * \brief Returns a 32-bit synchsafe integer converted from a normal 32-bit integer.
 * \remarks Synchsafe integers appear in ID3 tags that are attached to an MP3 file.
 * \sa <a href="http://id3.org/id3v2.4.0-structure">ID3 tag version 2.4.0 - Main Structure</a>
 */
LIB_EXPORT inline uint32 toSynchsafeInt(uint32 normalInt)
{
    return    ((normalInt & 0x0000007fu)     )
            | ((normalInt & 0x00003f80u) << 1)
            | ((normalInt & 0x001fc000u) << 2)
            | ((normalInt & 0x0fe00000u) << 3);
}

/*!
 * \brief Returns a normal 32-bit integer converted from a 32-bit synchsafe integer.
 * \remarks Synchsafe integers appear in ID3 tags that are attached to an MP3 file.
 * \sa <a href="http://id3.org/id3v2.4.0-structure">ID3 tag version 2.4.0 - Main Structure</a>
 */
LIB_EXPORT inline uint32 toNormalInt(uint32 synchsafeInt)
{
    return    ((synchsafeInt & 0x0000007fu)     )
            | ((synchsafeInt & 0x00007f00u) >> 1)
            | ((synchsafeInt & 0x007f0000u) >> 2)
            | ((synchsafeInt & 0x7f000000u) >> 3);
}

}

#endif // CONVERSION_UTILITIES_CONVERT_H

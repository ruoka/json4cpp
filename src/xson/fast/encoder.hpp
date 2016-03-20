#pragma once

#include <ostream>
#include <iterator>
#include "xson/trace.hpp"

namespace xson {
namespace fast {

class encoder
{
public:

    encoder(std::ostream& os) : m_os{os}
    {}

    void encode(std::uint8_t b);

    void encode(std::uint32_t in);

    void encode(std::int32_t i);

    void encode(std::uint64_t i);

    void encode(std::int64_t i);

    void encode(const std::string& str);

private:

    std::ostream& m_os;
};

inline void encoder::encode(std::uint8_t b)
{
    TRACE("std::uint8_t");
    m_os.put(b);
}

inline void encoder::encode(std::uint32_t i)
{
    TRACE("std::uint32_t");
    encode(static_cast<std::uint64_t>(i));
}

inline void encoder::encode(std::int32_t i)
{
    TRACE("std::uint32_t");
    encode(static_cast<std::int64_t>(i));
}

inline unsigned size(std::uint64_t i)
{
   if (i < 0x000000000000080ull) return 1; // 128
   if (i < 0x000000000004000ull) return 2; // 16384
   if (i < 0x000000000200000ull) return 3; // 2097152
   if (i < 0x000000010000000ull) return 4; // 268435456
   if (i < 0x000000800000000ull) return 5; // 34359738368
   if (i < 0x000040000000000ull) return 6; // 4398046511104
   if (i < 0x002000000000000ull) return 7; // 562949953421312
   if (i < 0x100000000000000ull) return 8; // 72057594037927936
   return 9;
}

inline void encoder::encode(std::uint64_t i)
{
    TRACE("std::uint64_t");
    switch(size(i))
    {
    // Shifts are logical (unsigned)
    case 9: m_os.put((i >> 56) & 0x7f);
    case 8: m_os.put((i >> 49) & 0x7f);
    case 7: m_os.put((i >> 42) & 0x7f);
    case 6: m_os.put((i >> 35) & 0x7f);
    case 5: m_os.put((i >> 28) & 0x7f);
    case 4: m_os.put((i >> 21) & 0x7f);
    case 3: m_os.put((i >> 14) & 0x7f);
    case 2: m_os.put((i >>  7) & 0x7f);
    case 1: m_os.put((i & 0x7f) | 0x80);
    }
}

inline unsigned size(std::int64_t i)
{
    if (i < 0)
    {
        if (i >= 0x00000000ffffffc0ll) return 1; // -64
        if (i >= 0x00000000ffffe000ll) return 2; // -8192
        if (i >= 0x00000000fff00000ll) return 3; // -1048576
        if (i >= 0x00000000f8000000ll) return 4; // -134217728
        if (i >= 0xfffffffc00000000ll) return 5; // -17179869184
        if (i >= 0xfffffe0000000000ll) return 6; // -2199023255552
        if (i >= 0xffff000000000000ll) return 7; // -281474976710656
        if (i >= 0xff80000000000000ll) return 8; // -36028797018963968
    }
    else
    {
        if (i <  0x00000000000040ll) return 1; // 64
        if (i <  0x00000000002000ll) return 2; // 8192
        if (i <  0x00000000100000ll) return 3; // 1048576
        if (i <  0x00000008000000ll) return 4; // 134217728
        if (i <  0x00000400000000ll) return 5; // 17179869184
        if (i <  0x00020000000000ll) return 6; // 2199023255552
        if (i <  0x01000000000000ll) return 7; // 281474976710656
        if (i <  0x80000000000000ll) return 8; // 36028797018963968
    }
    return 9;
}

inline void encoder::encode(std::int64_t i)
{
    TRACE("std::int64_t");
    switch(size(i))
    {
    // Shifts are arithmetic (signed)
    // The sign bit of data will be copied on right shifts
    case 9: m_os.put((i >> 56) & 0x7f);
    case 8: m_os.put((i >> 49) & 0x7f);
    case 7: m_os.put((i >> 42) & 0x7f);
    case 6: m_os.put((i >> 35) & 0x7f);
    case 5: m_os.put((i >> 28) & 0x7f);
    case 4: m_os.put((i >> 21) & 0x7f);
    case 3: m_os.put((i >> 14) & 0x7f);
    case 2: m_os.put((i >>  7) & 0x7f);
    case 1: m_os.put((i & 0x7f) | 0x80);
    }
}

inline void encoder::encode(const std::string& str)
{
    TRACE("std::string");
    auto head = str.cbegin();
    auto tail = head + (str.size() - 1);
    std::copy(head, tail, std::ostream_iterator<char>(m_os));
    m_os.put(*tail | 0x80);
}

} // namespace fast
} // namespace xson

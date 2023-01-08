#pragma once

#include <ostream>
#include <iterator>
#include <cmath>
#include "xson/trace.hpp"

namespace xson::fast {

inline void encode(std::ostream& os, std::uint8_t b)
{
    TRACE("std::uint8_t");
    os.put(b);
}

template<typename T> requires std::is_enum_v<T>
void encode(std::ostream& os, T e)
{
    encode(os,static_cast<std::uint8_t>(e));
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

inline void encode(std::ostream& os, std::uint64_t i)
{
    TRACE("std::uint64_t");
    switch(size(i))
    {
    // Shifts are logical (unsigned)
    case 9: os.put((i >> 56) & 0x7f);
    case 8: os.put((i >> 49) & 0x7f);
    case 7: os.put((i >> 42) & 0x7f);
    case 6: os.put((i >> 35) & 0x7f);
    case 5: os.put((i >> 28) & 0x7f);
    case 4: os.put((i >> 21) & 0x7f);
    case 3: os.put((i >> 14) & 0x7f);
    case 2: os.put((i >>  7) & 0x7f);
    case 1: os.put((i & 0x7f) | 0x80);
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"

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

#pragma clang diagnostic pop

inline void encode(std::ostream& os, std::int64_t i)
{
    TRACE("std::int64_t");
    switch(size(i))
    {
    // Shifts are arithmetic (signed)
    // The sign bit of data will be copied on right shifts
    case 9: os.put((i >> 56) & 0x7f);
    case 8: os.put((i >> 49) & 0x7f);
    case 7: os.put((i >> 42) & 0x7f);
    case 6: os.put((i >> 35) & 0x7f);
    case 5: os.put((i >> 28) & 0x7f);
    case 4: os.put((i >> 21) & 0x7f);
    case 3: os.put((i >> 14) & 0x7f);
    case 2: os.put((i >>  7) & 0x7f);
    case 1: os.put((i & 0x7f) | 0x80);
    }
}

inline void encode(std::ostream& os, std::uint32_t i)
{
    TRACE("std::uint32_t");
    encode(os,static_cast<std::uint64_t>(i));
}

inline void encode(std::ostream& os, std::int32_t i)
{
    TRACE("std::uint32_t");
    encode(os,static_cast<std::int64_t>(i));
}

inline void encode(std::ostream& os, const std::string& str)
{
    TRACE("std::string");
    auto head = str.cbegin();
    auto tail = head + (str.size() - 1);
    std::copy(head, tail, std::ostream_iterator<char>(os));
    os.put(*tail | 0x80);
}

inline void encode(std::ostream& os, std::double_t d)
{
    union {
        std::double_t d64;
        std::uint64_t i64;
    } d2i;
    d2i.d64 = d;
    encode(os,d2i.i64);
}

inline void encode(std::ostream& os, bool b)
{
    if(b)
        encode(os,std::uint8_t{'\x01'});
    else
        encode(os,std::uint8_t{'\x00'});
}

inline void encode(std::ostream& os, const std::chrono::system_clock::time_point& d)
{
    using namespace std::chrono;
    const auto us = duration_cast<milliseconds>(d.time_since_epoch());
    encode(os,static_cast<std::uint64_t>(us.count()));
}

} // namespace xson::fast

#pragma once

#include <istream>
#include <chrono>
#include <cmath>
#include "xson/trace.hpp"

namespace xson::fast {

inline void decode(std::istream& is, std::uint8_t& byte)
{
    byte = is.get();
    TRACE(byte);
}

template<typename T> requires std::is_enum_v<T>
void decode(std::istream& is, T& e)
{
    std::uint8_t byte;
    decode(is,byte);
    e = static_cast<T>(byte);
    TRACE(e);
}

inline void decode(std::istream& is, std::uint32_t& i)
{
    i = 0u;
    for(std::uint8_t byte = is.get(); is; byte = is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
    TRACE(i);
}

inline void decode(std::istream& is, std::int32_t& i)
{
    i = (is.peek() & 0x40) ? -1 : 0;
    for(std::uint8_t byte = is.get(); is; byte = is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
    TRACE(i);
}

inline void decode(std::istream& is, std::uint64_t& i)
{
    i = 0ull;
    for(std::uint8_t byte = is.get(); is; byte = is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
    TRACE(i);
}

inline void decode(std::istream& is, std::int64_t& i)
{
    i = (is.peek() & 0x40) ? -1ll : 0ll;
    for(std::uint8_t byte = is.get(); is; byte = is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
    TRACE(i);
}

inline void decode(std::istream& is, std::string& str)
{
    str.clear();
    for(char byte = is.get(); is; byte = is.get())
    {
        str.push_back(byte & 0x7f);
        if(byte & 0x80)
            break;
    }
    TRACE(str);
}

inline void decode(std::istream& is, std::double_t& d)
{
    union {
        std::uint64_t i64;
        std::double_t d64;
    } i2d;
    decode(is,i2d.i64);
    d = i2d.d64;
    TRACE(d);
}

inline void decode(std::istream& is, bool& b)
{
    std::uint8_t byte;
    decode(is,byte);
    b = static_cast<std::uint8_t>(byte);
    TRACE(b);
}

inline void decode(std::istream& is, std::chrono::system_clock::time_point& dt)
{
    using namespace std::chrono;
    std::uint64_t i64;
    decode(is,i64);
    dt = system_clock::time_point{milliseconds{i64}};
    TRACE(i64);
}

} // namespace xson::fast

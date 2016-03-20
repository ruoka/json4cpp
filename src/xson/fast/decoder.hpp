#pragma once

#include <istream>

namespace xson {
namespace fast {

class decoder
{
public:

    decoder(std::istream& is) : m_is{is}
    {}

    void decode(std::uint8_t& byte);

    void decode(std::uint32_t& i);

    void decode(std::int32_t& i);

    void decode(std::uint64_t& i);

    void decode(std::int64_t& i);

    void decode(std::string& str);

protected:

    std::istream& m_is;
};

inline void decoder::decode(std::uint8_t& byte)
{
    byte = m_is.get();
}

inline void decoder::decode(std::uint32_t& i)
{
    i = 0u;
    for(std::uint8_t byte = m_is.get(); m_is; byte = m_is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
}

inline void decoder::decode(std::int32_t& i)
{
    i = (m_is.peek() & 0x40) ? -1 : 0;
    for(std::uint8_t byte = m_is.get(); m_is; byte = m_is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
}

inline void decoder::decode(std::uint64_t& i)
{
    i = 0ull;
    for(std::uint8_t byte = m_is.get(); m_is; byte = m_is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
}

inline void decoder::decode(std::int64_t& i)
{
    i = (m_is.peek() & 0x40) ? -1ll : 0ll;
    for(std::uint8_t byte = m_is.get(); m_is; byte = m_is.get())
    {
        i = (i << 7) | (byte & 0x7f);
        if(byte & 0x80)
            break;
    }
}

inline void decoder::decode(std::string& str)
{
    str.clear();
    for(char byte = m_is.get(); m_is; byte = m_is.get())
    {
        str.push_back(byte & 0x7f);
        if(byte & 0x80)
            break;
    }
}

} // namespace fast
} // namespace xson

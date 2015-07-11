#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson {
namespace bson {

std::ostream& operator << (std::ostream& os, const object& obj);

class decoder
{
public:

    using const_iterator = std::vector<char>::const_iterator;

    using pointer = std::vector<char>::pointer;

    using size_type = std::vector<char>::size_type;

    decoder() : m_buffer{}
    {}

    decoder(const object& obj) : decoder()
    {
        decode(obj);
    }

    const_iterator cbegin() const
    {
        return m_buffer.cbegin();
    }

    const_iterator cend() const
    {
        return m_buffer.end();
    }

    size_type size() const
    {
        return m_buffer.size();
    }

    pointer data()
    {
        return m_buffer.data();
    }

//private:

    template <typename T>
    void decode(T);

    void decode(const string_type& str, bool csting = false);

    void decode(const object& obj);

    void put(char b);

    std::vector<char> m_buffer;
};

template <>
inline void decoder::decode(int32_type i)
{
    TRACE("(int32)");
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
}

template <>
inline void decoder::decode(int64_type i)
{
    TRACE("(int64)");
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
    put((i >> 32) & 0xFF);
    put((i >> 40) & 0xFF);
    put((i >> 48) & 0xFF);
    put((i >> 56) & 0xFF);
}

template <>
inline void decoder::decode(double_type d)
{
    TRACE("(double)");
    union{
        double_type d64;
        int64_type i64;
    } d2i;
    d2i.d64 = d;
    decode(d2i.i64);
}
template <>
inline void decoder::decode(boolean_type b)
{
    TRACE("(bool)");
    if(b) put('\x01');
    else  put('\x00');
}

template <>
inline void decoder::decode(date_type d)
{
    using namespace std::chrono;
    TRACE("(date)");
    const int64_type i = duration_cast<milliseconds>(d.time_since_epoch()).count();
    decode(i);
}

template <>
inline void decoder::decode(null_type b)
{
    TRACE("(null)");
}

template <>
inline void decoder::decode(const string_type str)
{
    TRACE("(string)");
    decode(static_cast<int32_type>(str.size()+1)); // bytes
    for(char b : str)                              // data
        put(b);
    put('\x00');                                   // 0
}

template <>
inline void decoder::decode(xson::type t)
{
    TRACE("(type)");
    decode(static_cast<int32_t>(t));
}

inline void decoder::decode(const string_type& str, bool csting)
{
    TRACE("(string)    " << boolalpha << csting);
    if(!csting) decode(static_cast<int32_type>(str.size()+1)); // bytes
    for(char b : str)            // data
        put(b);
    put('\x00');                      // 0
}

inline void decoder::decode(const object& obj)
{
    TRACE("(object)");

    switch(obj.type())
    {
    case type::object:
    case type::array:
    {
        const int32_type head = size();

        // We'll leave the length field empty for the time being...

        decode<int32_type>(0);       // length = 0

        for(const auto& o : obj)
        {
            decode(o.second.type()); // name
            decode(o.first, true);   // type
            decode(o.second);        // object
        }

        put('\x00');                 // tailing 0

        // Now we'll fix the length field...

        const int32_type tail = size();
        decoder bytes;
        bytes.decode<int32_type>(tail - head - 4);
        std::copy(bytes.cbegin(), bytes.cend(), m_buffer.begin()+head);
    }
        break;

    case type::number:
        decode<double_type>(obj);
        break;

    case type::string:
        decode<string_type>(obj);
        break;

    case type::boolean:
        decode<boolean_type>(obj);
        break;

    case type::date:
        decode<date_type>(obj);

    case type::null:
        decode<null_type>(nullptr);
        break;

    case type::int32:
        decode<int32_type>(obj);
        break;

    case type::int64:
        decode<int64_type>(obj);
        break;

    default:
        assert(false && "FIXME!");
    }
}

inline void decoder::put(char b)
{
    m_buffer.emplace_back(b);
    TRACE("(byte)     " << setw(5) << m_buffer.size() << setw(5) << b << " " << setw(5) << hex << uppercase << (int)b << dec);
}

inline std::ostream& operator << (std::ostream& os, const object& obj)
{
    decoder dc{obj};
    os.write(dc.data(), dc.size());
    os.flush();
    return os;
}

} // namespace bson
} // namespace xson

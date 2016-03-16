#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson {
namespace bson {

std::ostream& operator << (std::ostream& os, const object& obj);

class encoder
{
public:

    using const_iterator = std::vector<char>::const_iterator;

    using pointer = std::vector<char>::pointer;

    using size_type = std::vector<char>::size_type;

    encoder() : m_buffer{}
    {}

    encoder(const object& obj) : encoder()
    {
        encode(obj);
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
    void encode(T);

    void encode(const string_type& str, bool csting = false);

    void encode(const object& obj);

    void put(char b);

    std::vector<char> m_buffer;
};

template <>
inline void encoder::encode(int32_type i)
{
    TRACE("(int32)");
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
}

template <>
inline void encoder::encode(int64_type i)
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
inline void encoder::encode(double_type d)
{
    TRACE("(double)");
    union{
        double_type d64;
        int64_type i64;
    } d2i;
    d2i.d64 = d;
    encode(d2i.i64);
}

template <>
inline void encoder::encode(boolean_type b)
{
    TRACE("(bool)");
    if(b) put('\x01');
    else  put('\x00');
}

template <>
inline void encoder::encode(date_type d)
{
    using namespace std::chrono;
    TRACE("(date)");
    const int64_type i = duration_cast<milliseconds>(d.time_since_epoch()).count();
    encode(i);
}

template <>
inline void encoder::encode(null_type b)
{
    TRACE("(null)");
}

template <>
inline void encoder::encode(const string_type str)
{
    TRACE("(string)");
    encode(static_cast<int32_type>(str.size()+1)); // bytes
    for(char b : str)                              // data
        put(b);
    put('\x00');                                   // 0
}

template <>
inline void encoder::encode(xson::type t)
{
    TRACE("(type)");
    put(static_cast<char>(t));
}

inline void encoder::encode(const string_type& str, bool csting)
{
    TRACE("(string)    " << boolalpha << csting);
    if(!csting) encode(static_cast<int32_type>(str.size()+1)); // bytes
    for(char b : str)            // data
        put(b);
    put('\x00');                      // 0
}

inline void encoder::encode(const object& obj)
{
    TRACE("(object)");

    switch(obj.type())
    {
    case type::object:
    case type::array:
    {
        const int32_type head = size();

        // We'll leave the length field empty for the time being...

        encode<int32_type>(0);       // length = 0

        for(const auto& o : obj)
        {
            encode(o.second.type()); // type
            encode(o.first, true);   // name
            encode(o.second);        // object
        }

        put('\x00');                 // tailing 0

        // Now we'll fix the length field...

        const int32_type tail = size();
        encoder bytes;
        bytes.encode<int32_type>(tail - head);
        std::copy(bytes.cbegin(), bytes.cend(), m_buffer.begin()+head);
    }
        break;

    case type::number:
        encode<double_type>(obj);
        break;

    case type::string:
        encode<string_type>(obj);
        break;

    case type::boolean:
        encode<boolean_type>(obj);
        break;

    case type::date:
        encode<date_type>(obj);

    case type::null:
        encode<null_type>(nullptr);
        break;

    case type::int32:
        encode<int32_type>(obj);
        break;

    case type::int64:
        encode<int64_type>(obj);
        break;

    default:
        assert(false && "FIXME!");
    }
}

inline void encoder::put(char b)
{
    m_buffer.emplace_back(b);
    TRACE("(byte)     " << setw(5) << m_buffer.size() << setw(5) << b << " " << setw(5) << hex << uppercase << (int)b << dec);
}

inline std::ostream& operator << (std::ostream& os, const object& obj)
{
    encoder dc{obj};
    os.write(dc.data(), dc.size());
    os.flush();
    return os;
}

} // namespace bson
} // namespace xson

#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson::bson
{

std::ostream& operator << (std::ostream& os, const object& ob);

class encoder
{
public:

    using const_iterator = std::vector<char>::const_iterator;

    using const_pointer = std::vector<char>::const_pointer;

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

    const_pointer data() const
    {
        return m_buffer.data();
    }

//private:

    template <typename T>
    void encode(T);

    void encode(const std::string_t& str, bool csting = false);

    void encode(const object& obj);

    void put(char b);

    std::vector<char> m_buffer;
};

template <>
inline void encoder::encode(std::int32_t i)
{
    TRACE("(int32)");
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
}

template <>
inline void encoder::encode(std::int64_t i)
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
inline void encoder::encode(std::double_t d)
{
    TRACE("(double)");
    union
    {
        std::double_t d64;
        std::int64_t i64;
    } d2i;
    d2i.d64 = d;
    encode(d2i.i64);
}

template <>
inline void encoder::encode(std::bool_t b)
{
    TRACE("(bool)");
    if(b) put('\x01');
    else  put('\x00');
}

template <>
inline void encoder::encode(std::datetime_t d)
{
    TRACE("(date)");
    using namespace std::chrono;
    const auto us = duration_cast<milliseconds>(d.time_since_epoch());
    encode(static_cast<std::int64_t>(us.count()));
}

template <>
inline void encoder::encode(std::nullptr_t b)
{
    TRACE("(null)");
}

template <>
inline void encoder::encode(const std::string_t str)
{
    TRACE("(string)");
    encode(static_cast<std::int32_t>(str.size()+1)); // bytes
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

inline void encoder::encode(const std::string_t& str, bool csting)
{
    TRACE("(string)    " << boolalpha << csting);
    if(!csting) encode(static_cast<std::int32_t>(str.size()+1)); // bytes
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
        const auto head = size();

        // We'll leave the length field empty for the time being...

        encode<std::int32_t>(0);       // length = 0

        for(const auto& o : obj)
        {
            encode(o.second.type()); // type
            encode(o.first, true);   // name
            encode(o.second);        // object
        }

        put('\x00');                 // tailing 0

        // Now we'll fix the length field...

        const auto tail = size();
        encoder bytes;
        bytes.encode<std::int32_t>(tail - head);
        std::copy(bytes.cbegin(), bytes.cend(), m_buffer.begin()+head);
    }
        break;

    case type::number:
        encode<std::double_t>(obj);
        break;

    case type::string:
        encode<std::string_t>(obj);
        break;

    case type::boolean:
        encode<std::bool_t>(obj);
        break;

    case type::date:
        encode<std::datetime_t>(obj);

    case type::null:
        encode<std::nullptr_t>(nullptr);
        break;

    case type::int32:
        encode<std::int32_t>(obj);
        break;

    case type::int64:
        encode<std::int64_t>(obj);
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

inline std::ostream& operator << (std::ostream& os, const xson::object& ob)
{
    const auto e = encoder{ob};
    os.write(e.data(), e.size());
    os.flush();
    return os;
}

} // namespace xson::bson

#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson {
namespace bson {

std::istream& operator >> (std::istream& os, object& ob);

using int32_type = std::int32_t;                           // \x10
using int64_type = std::int64_t;                           // \x12

class encoder
{
public:

    encoder(std::istream& is) : m_is{is}
    {}

    int32_type encode(object& ob);

private:

    template <typename T>
    int32_type encode(object& ob);

    std::istream& m_is;
};

template <typename T>
inline int32_type encoder::encode(object& ob)
{
    T val;
    m_is.read(reinterpret_cast<char*>(&val), sizeof(val));
    ob.value(val);
    TRACE("   val: "  << ob.value() << "; size: " << sizeof(val));
    return m_is.gcount();
}

template<>
inline int32_type encoder::encode<std::chrono::system_clock::time_point>(object& ob)
{
    using namespace std::chrono;
    int64_type val1;
    m_is.read(reinterpret_cast<char*>(&val1), sizeof(val1));
    const auto val2 = system_clock::time_point{milliseconds{val1}};
    ob.value(val2);
    TRACE("   val: "  << ob.value() << "; size: " << sizeof(val1));
    return m_is.gcount();
}

template<>
inline int32_type encoder::encode<std::nullptr_t>(object& ob)
{
    std::nullptr_t val;
    ob.value(val);
    TRACE("   val: "  << ob.value() << "; size: " << 0);
    return 0;
}

template<>
inline int32_type encoder::encode<std::string>(object& ob)
{
    int32_type bytes;
    m_is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes + 1;

    std::string val;
    while(--bytes)
        val += m_is.get();

    m_is.ignore(1);
    --bytes;

    ob.value(val);

    TRACE("   val: "  << ob.value() << "; size: " << val.size());
    return length;
}

inline int32_type encoder::encode(object& parent)
{
    int32_type bytes;
    m_is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes;

    TRACE("length:   " << length);
    TRACE("bytes:    " << bytes);

    while(bytes > 1)
    {
        object::type type;
        m_is.read(reinterpret_cast<char*>(&type), sizeof(type));
        bytes -= m_is.gcount();

        TRACE("type: " << type);

        std::string name;
        std::getline(m_is, name, '\x00');
        bytes -= name.size();
        --bytes;

        TRACE("key:  " << name);

        auto& child = parent[name];

        switch(type)
        {
        case object::type::number:
            bytes -= encode<double>(child);
            break;
        case object::type::string:
            bytes -= encode<std::string>(child);
            break;
        case object::type::object:
        case object::type::array:
            bytes -= encode(child);
            child.value(type);
            break;
        case object::type::boolean:
            bytes -= encode<bool>(child);
            break;
        case object::type::date:
            bytes -= encode<std::chrono::system_clock::time_point>(child);
            break;
        case object::type::null:
            bytes -= encode<std::nullptr_t>(child);
            break;
        case object::type::int32:
            bytes -= encode<std::int32_t>(child);
            break;
        case object::type::int64:
            bytes -= encode<std::int64_t>(child);
            break;
        default:
            assert(false && "FIXME");
            break;
        }

        TRACE("bytes: " << bytes);
    }

    m_is.ignore(1);
    --bytes;

    TRACE("bytes: " << bytes);

    return length;
}

inline std::istream& operator >> (std::istream& is, object& ob)
{
    encoder{is}.encode(ob);
    return is;
}

} // namespace bson
} // namespace xson

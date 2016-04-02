#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson::bson
{

std::istream& operator >> (std::istream& os, object& ob);

using int32_type = std::int32_t;                           // \x10
using int64_type = std::int64_t;                           // \x12

class decoder
{
public:

    decoder(std::istream& is) : m_is{is}
    {}

    int32_type decode(object& ob);

private:

    template <typename T>
    int32_type decode(object& ob);

    std::istream& m_is;
};

template <typename T>
inline int32_type decoder::decode(object& ob)
{
    auto val = T{};
    m_is.read(reinterpret_cast<char*>(&val), sizeof(val));
    ob.value(val);
    TRACE("val: "  << ob.value() << "; size: " << sizeof(val));
    return m_is.gcount();
}

template<>
inline int32_type decoder::decode<std::chrono::system_clock::time_point>(object& ob)
{
    using namespace std::chrono;
    auto val1 = int64_type{};
    m_is.read(reinterpret_cast<char*>(&val1), sizeof(val1));
    const auto val2 = system_clock::time_point{milliseconds{val1}};
    ob.value(val2);
    TRACE("val: "  << ob.value() << "; size: " << sizeof(val1));
    return m_is.gcount();
}

template<>
inline int32_type decoder::decode<std::nullptr_t>(object& ob)
{
    auto val = nullptr;
    ob.value(val);
    TRACE("val: "  << ob.value() << "; size: " << 0);
    return 0;
}

template<>
inline int32_type decoder::decode<std::string>(object& ob)
{
    auto bytes = int32_type{0};    
    m_is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes;

    auto val = std::string{};
    while(--bytes)
        val += m_is.get();

    m_is.ignore(1);
    --bytes;

    ob.value(val);

    TRACE("val: "  << ob.value() << "; size: " << val.size());
    return length;
}

inline int32_type decoder::decode(object& parent)
{
    auto bytes = int32_type{0};
    m_is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = bytes;
    bytes -= sizeof(bytes);

    TRACE("length: " << length);
    TRACE("bytes: " << bytes);

    while(bytes > 1)
    {
        auto type = xson::type{};
        m_is.read(reinterpret_cast<char*>(&type), sizeof(type));
        --bytes;

        TRACE("type: " << type);

        auto name = std::string{};
        std::getline(m_is, name, '\x00');
        bytes -= name.size();
        --bytes;

        TRACE("key: " << name);

        auto& child = parent[name];

        switch(type)
        {
        case type::number:
            bytes -= decode<double>(child);
            break;
        case type::string:
            bytes -= decode<std::string>(child);
            break;
        case type::object:
        case type::array:
            bytes -= decode(child);
            child.type(type);
            break;
        case type::boolean:
            bytes -= decode<bool>(child);
            break;
        case type::date:
            bytes -= decode<std::chrono::system_clock::time_point>(child);
            break;
        case type::null:
            bytes -= decode<std::nullptr_t>(child);
            break;
        case type::int32:
            bytes -= decode<std::int32_t>(child);
            break;
        case type::int64:
            bytes -= decode<std::int64_t>(child);
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
    decoder{is}.decode(ob);
    return is;
}

} // namespace xson::bson

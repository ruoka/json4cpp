#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson::bson
{

class decoder
{
public:

    decoder(std::istream& is) : m_is{is}
    {}

    std::int32_t decode(object& ob);

private:

    template <typename T>
    std::int32_t decode(object& ob);

    std::istream& m_is;
};

template <typename T>
inline std::int32_t decoder::decode(object& ob)
{
    auto val = T{};
    m_is.read(reinterpret_cast<char*>(&val), sizeof(val));
    ob.value(val);
    TRACE("val: "  << ob.value() << "; size: " << sizeof(val));
    return m_is.gcount();
}

template<>
inline std::int32_t decoder::decode<std::chrono::system_clock::time_point>(object& ob)
{
    using namespace std::chrono;
    auto val1 = std::int64_t{};
    m_is.read(reinterpret_cast<char*>(&val1), sizeof(val1));
    const auto val2 = system_clock::time_point{milliseconds{val1}};
    ob.value(val2);
    TRACE("val: "  << ob.value() << "; size: " << sizeof(val1));
    return m_is.gcount();
}

template<>
inline std::int32_t decoder::decode<std::nullptr_t>(object& ob)
{
    auto val = nullptr;
    ob.value(val);
    TRACE("val: "  << ob.value() << "; size: " << 0);
    return 0;
}

template<>
inline std::int32_t decoder::decode<std::string>(object& ob)
{
    auto bytes = std::int32_t{0};
    m_is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes;

    auto val = std::string_t{};
    while(--bytes)
        val += m_is.get();

    m_is.ignore(1);
    --bytes;

    ob.value(val);

    TRACE("val: "  << ob.value() << "; size: " << val.size());
    return length;
}

inline std::int32_t decoder::decode(object& parent)
{
    auto bytes = std::int32_t{0};
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
            bytes -= decode<std::double_t>(child);
            break;
        case type::string:
            bytes -= decode<std::string_t>(child);
            break;
        case type::object:
        case type::array:
            bytes -= decode(child);
            child.type(type);
            break;
        case type::boolean:
            bytes -= decode<std::bool_t>(child);
            break;
        case type::date:
            bytes -= decode<std::datetime_t>(child);
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

inline auto& operator >> (std::istream& is, object& ob)
{
    decoder{is}.decode(ob);
    return is;
}

} // namespace xson::bson

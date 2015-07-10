#pragma once

#include "xson/object.hpp"
#include "xson/trace.hpp"

namespace xson {
namespace bson {

std::istream& operator >> (std::istream& os, object& obj);

using int32_type = std::int32_t;                           // \x10
using int64_type = std::int64_t;                           // \x12

class encoder
{
public:

    encoder() = delete;

    static void encode(std::istream& is, object& obj)
    {
        encode_document(is, obj);
    }

private:

    template <typename T>
    static int32_type encode(std::istream& is, object& obj);

    static int32_type encode_document(std::istream& is, object& obj);

};

template <typename T>
inline int32_type encoder::encode(std::istream& is, object& obj)
{
    T val;
    is.read(reinterpret_cast<char*>(&val), sizeof(val));
    obj.m_value = object::to_value(val);
    obj.m_type = object::to_type(val);

    TRACE("   val: "  << obj.m_value << "; size: " << sizeof(val));

    return is.gcount();
}

template<>
inline int32_type encoder::encode<std::chrono::system_clock::time_point>(std::istream& is, object& obj)
{
    using namespace std::chrono;
    int64_type val1;
    is.read(reinterpret_cast<char*>(&val1), sizeof(val1));
    const auto val2 = system_clock::time_point{milliseconds{val1}};
    obj.m_value = object::to_value(val2);
    obj.m_type = object::to_type(val2);

    TRACE("   val: "  << obj.m_value << "; size: " << sizeof(val1));

    return is.gcount();
}

template<>
inline int32_type encoder::encode<std::nullptr_t>(std::istream& is, object& obj)
{
    std::nullptr_t val;
    obj.m_value = object::to_value(val);
    obj.m_type = object::to_type(val);

    TRACE("   val: "  << obj.m_value << "; size: " << 0);

    return 0;
}

template<>
inline int32_type encoder::encode<std::string>(std::istream& is, object& obj)
{
    int32_type bytes;
    is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes + 1;

    std::string val;
    while(--bytes)
        val += is.get();

    is.ignore(1);
    --bytes;

    obj.m_value = object::to_value(val);
    obj.m_type = object::to_type(val);

    TRACE("   val: "  << obj.m_value << "; size: " << val.size());

    return length;
}

inline int32_type encoder::encode_document(std::istream& is, object& obj)
{
    int32_type bytes;
    is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes;

    TRACE("length: " << length);
    TRACE("bytes: "  << bytes);

    while(bytes > 1)
    {
        object::type type;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        bytes -= is.gcount();

        std::string name;
        std::getline(is, name, '\x00');
        bytes -= name.size();
        --bytes;

        TRACE("key: " << name);

        switch(type)
        {
        case object::type::number:
            bytes -= encode<double>(is, obj.m_objects[name]);
            break;
        case object::type::string:
            bytes -= encode<std::string>(is, obj.m_objects[name]);
            break;
        case object::type::object:
        case object::type::array:
            bytes -= encode_document(is, obj.m_objects[name]);
            obj.m_objects[name].m_type = type;
            break;
        case object::type::boolean:
            bytes -= encode<bool>(is, obj.m_objects[name]);
            break;
        case object::type::date:
            bytes -= encode<std::chrono::system_clock::time_point>(is, obj.m_objects[name]);
            break;
        case object::type::null:
            bytes -= encode<std::nullptr_t>(is, obj.m_objects[name]);
            break;
        case object::type::int32:
            bytes -= encode<std::int32_t>(is, obj.m_objects[name]);
            break;
        case object::type::int64:
            bytes -= encode<std::int64_t>(is, obj.m_objects[name]);
            break;
        default:
            assert(false && "This type is not supported yet");
            break;
        }

        TRACE("bytes: " << bytes);
    }

    is.ignore(1);
    --bytes;

    TRACE("bytes: " << bytes);

    return length;
}

inline std::istream& operator >> (std::istream& is, object& obj)
{
    encoder::encode(is, obj);
    return is;
}

} // namespace bson
} // namespace xson

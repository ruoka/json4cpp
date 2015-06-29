#pragma once

#include <cstdint>
#include <initializer_list>
#include <iosfwd>

namespace bson
{
    struct document;
    struct list;
    struct element;
    struct array;
    struct binary;
    struct objectid;

    using byte_type = std::uint8_t;
    using int32_type = std::int32_t;
    using int64_type = std::int64_t;
    using double_type = double;
    using string_type = std::string;
    using cstring_type = char*;
    using document_type = document;
    using array_type = array;
    using binary_type = binary;
    using objectid_type = objectid;
    using boolean_type = bool;
    using null_type = std::nullptr_t;

    template <typename T> constexpr int32_type type(const T&)
    {
        return 0x06;
    };

    template <> int32_type constexpr type(const double_type&)
    {
        return 0x01;
    };

    template <> int32_type constexpr type(const string_type&)
    {
        return 0x02;
    };

    template <> int32_type constexpr type(const document_type&)
    {
        return 0x03;
    };

    template <> int32_type constexpr type(const array_type&)
    {
        return 0x04;
    };

    template <> int32_type constexpr type(const binary_type&)
    {
        return 0x05;
    };

    template <> int32_type constexpr type(const objectid_type&)
    {
        return 0x07;
    };

    template <> int32_type constexpr type(const boolean_type& b)
    {
        return !b ? 0x08 : 0x09;
    };

    template <> int32_type constexpr type(const null_type&)
    {
        return 0x0A;
    };

    template <> int32_type constexpr type(const int32_type&)
    {
        return 0x10;
    };

    template <> int32_type constexpr type(const int64_type&)
    {
        return 0x12;
    };

    struct objectid
    {
        byte_type epoch[4];
        byte_type machine[3];
        byte_type process[2];
        byte_type counter[3];
    };

    struct binary
    {
        int32_type size;
        int32_type subtype;
        byte_type* bytes;
    };

    struct element
    {
        template <typename V>
        element(const std::string& n, const V& v) :
        type{bson::type(v)},
        name{n}
        {}
        int32_type type;
        string_type name;
        union
        {
            double_type double_value;
//            document_type document_value;
//            array_type array_value;
            binary_type binary_value;
            null_type null_value;
            objectid_type objectid_value;
        };
    };

    struct document
    {
        document()
        {}
        template <typename V>
        document(const std::string& key, const V& value)
        {}
        document(std::initializer_list<element> il) : document()
        {}
        friend std::ostream& operator << (std::ostream& os, const document&)
        {
            return os;
        }
    };

    struct array : public document
    {
        array() : document()
        {}
        template <typename V>
        array(std::initializer_list<V> il) : array()
        {}
    };

} // namespace bson

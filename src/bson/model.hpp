#pragma once

#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include "bson/decoder.hpp"

namespace bson
{
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

    struct element : public decoder
    {
        template <typename V>
        element(const std::string& name, const V& value)
        {
            decode(type(value));
            decode(name);
            decode(value);
        }
    };

    struct array : public decoder
    {
        array()
        {
            decode(int32_type{0x04});
            decode(int32_type{0});
        }
        template <typename V>
        array(std::initializer_list<V> il) : array()
        {
            for(auto& i : il)
                decode(i);
        }
    };

    struct document : public decoder
    {
        document()
        {
            decode(int32_type{0x04});
            decode(int32_type{0});
        }
        template <typename V>
        document(const std::string& key, const V& value)
        {
            decode(element{key, value});
        }
        document(std::initializer_list<element> il) : document()
        {
            for(auto& i : il)
                decode(i);
        }
        friend std::ostream& operator << (std::ostream& os, const document&)
        {
            return os;
        }
    };

} // namespace bson

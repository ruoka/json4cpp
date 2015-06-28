#pragma once

#include <cstdint>
#include <initializer_list>
#include <sstream>

namespace json
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
        element(const std::string& key, const V& value)
        {
            serial << "\"" << key << "\":";
            if (std::is_same<V,std::string>::value) serial << "\"";
            serial << value;
            if (std::is_same<V,std::string>::value) serial << "\"";
        }
        friend std::ostream& operator << (std::ostream& os, const element& e)
        {
            return os << e.serial.str();
        }
        std::ostringstream serial;
    };

    struct document
    {
        document()
        {
            serial << "{}";
        }
        template <typename V>
        document(const std::string& key, const V& value)
        {
            serial << '{' << element{key, value} << '}';
        }
        document(std::initializer_list<element> il)
        {
            serial << '{';
            for(auto it = il.begin(); it != il.end(); ++it)
            {
                if (it != il.begin()) serial << ',';
                serial << *it;
            }
            serial << '}';
        }
        friend std::ostream& operator << (std::ostream& os, const document& d)
        {
            return os << d.serial.str();
        }
        std::ostringstream serial;
    };

    struct array
    {
        array()
        {
            serial << "{}";
        }
        template <typename V>
        array(std::initializer_list<V> il)
        {
            serial << '[';
            for(auto it = il.begin(); it != il.end(); ++it)
            {
                if (it != il.begin()) serial << ',';
                serial << *it;
            }
            serial << ']';
        }
        friend std::ostream& operator << (std::ostream& os, const array& a)
        {
            return os << a.serial.str();
        }
        std::ostringstream serial;
    };

} // namespace json

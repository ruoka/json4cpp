#pragma once

#include <type_traits>

namespace bson
{

struct document;
struct element;
struct array;
struct binary;
struct objectid;

using byte_type = std::int8_t;
using int32_type = std::int32_t;
using int64_type = std::int64_t;
using double_type = double;
using element_type = element;
using string_type = std::string;
using cstring_type = std::string;
using document_type = document;
using array_type = array;
using binary_type = binary;
using objectid_type = objectid;
using boolean_type = bool;
using null_type = std::nullptr_t;

template <typename T> int32_type constexpr type(const T&)
{
    static_assert(std::is_void<T>::value, "This type is not supported");
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

} // namespace bson

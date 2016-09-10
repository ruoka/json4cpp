#pragma once

#include <ostream>
#include <string>
#include <chrono>
#include <array>
#include <vector>

namespace xson {

class object;

enum class type : std::uint8_t
{
    eod                   = '\x00',

    // json types
    number                = '\x01',
    string                = '\x02',
    object                = '\x03',
    array                 = '\x04',
    boolean               = '\x08',
    null                  = '\x0A',

    // + bson types
//  binary                = '\x05',
//  undefined             = '\x06', // Deprecated
//  objectid              = '\x07',
    date                  = '\x09',
//  regular_expression    = '\x0B',
//  db_pointer            = '\x0C', // Deprecated
//  javascript            = '\x0D',
//  deprecated            = '\x0E',
//  javascript_with_scope = '\x0F',
    int32                 = '\x10',
//  timestamp             = '\x11',
    int64                 = '\x12',
//  min_key               = '\xFF',
//  max_key               = '\x7F'
};

inline auto& operator << (std::ostream& os, type t)
{
    os << static_cast<int>(t);
    return os;
}

template <typename T> constexpr type to_type(const T&)
{
    static_assert(std::is_void<T>::value, "This type is not supported");
    return type::null;
}

template <> constexpr type to_type(const std::double_t&)
{
    return type::number;
}

template <> constexpr type to_type(const std::string_t&)
{
    return type::string;
}

template <> constexpr type to_type(const std::bool_t&)
{
    return type::boolean;
}

template <> constexpr type to_type(const std::nullptr_t&)
{
    return type::null;
}

template <> constexpr type to_type(const std::datetime_t&)
{
    return type::date;
}

template <> constexpr type to_type(const std::int32_t&)
{
    return type::int32;
}

template <> constexpr type to_type(const std::int64_t&)
{
    return type::int64;
}


template <typename T> struct is_object : std::false_type {};

template <> struct is_object<object> : std::true_type {};

template <typename T> constexpr bool is_object_v = is_object<T>::value;


template <typename T> struct is_value : std::false_type {};

template <> struct is_value<std::int32_t> : std::true_type {};

template <> struct is_value<std::int64_t> : std::true_type {};

template <> struct is_value<std::double_t> : std::true_type {};

template <> struct is_value<std::bool_t> : std::true_type {};

template <> struct is_value<std::string_t> : std::true_type {};

template <> struct is_value<std::datetime_t> : std::true_type {};

template <> struct is_value<std::nullptr_t> : std::true_type {};

template <typename T> constexpr bool is_value_v = is_value<T>::value;


template <typename T> struct is_value_array : std::false_type {};

template <typename T> struct is_value_array<std::initializer_list<T>> : is_value<T> {};

template <typename T> struct is_value_array<std::vector<T>> : is_value<T> {};

template <typename T, std::size_t N> struct is_value_array<std::array<T,N>> : is_value<T> {};

template <typename T, std::size_t N> struct is_value_array<T[N]> : is_value<T> {};

template <typename T> constexpr bool is_value_array_v = is_value_array<T>::value;


template <typename T> struct is_object_array : std::false_type {};

template <> struct is_object_array<std::initializer_list<object>> : std::true_type {};

template <> struct is_object_array<std::vector<object>> : std::true_type {};

template <std::size_t N> struct is_object_array<std::array<object,N>> : std::true_type {};

template <std::size_t N> struct is_object_array<object[N]> : std::true_type {};

template <typename T> constexpr bool is_object_array_v = is_object_array<T>::value;

} // namespace xson

#pragma once

#include <ostream>
#include <string>
#include <chrono>
#include <array>
#include <vector>

namespace xson {

class object;

using double_type = double;                                 // \x01
using string_type = std::string;                            // \x02
using document_type = object;                               // \x03
using array_type = object;                                  // \x04
//using binary_type = binary;                               // \x05
//using undefined_type = void;                              // \x06 — Deprecated
//using objectid_type = objectid;                           // \x07
using boolean_type = bool;                                  // \x08
using date_type = std::chrono::system_clock::time_point;    // \x09
using null_type = std::nullptr_t;                           // \x0A
//using regular_expression_type = regular_expression;       // \x0B
//using dbpointer_type = dbpointer;                         // \x0C — Deprecated
//using javascript_type = javascript;                       // \x0D
//using deprecated_type = void;                             // \x0E
//using javascript_with_scope_type = javascript_with_scope; // \x0F
using int32_type = std::int32_t;                            // \x10
//using timestamp_type = timestamp;                         // \x11
using int64_type = std::int64_t;                            // \x12

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

inline std::ostream& operator << (std::ostream& os, type t)
{
    os << static_cast<int>(t);
    return os;
}

template <typename T> constexpr type to_type(const T&)
{
    static_assert(std::is_void<T>::value, "This type is not supported");
    return type::null;
}

template <> constexpr type to_type(const double&)
{
    return type::number;
}

template <> constexpr type to_type(const std::string&)
{
    return type::string;
}

template <> constexpr type to_type(const bool&)
{
    return type::boolean;
}

template <> constexpr type to_type(const std::nullptr_t&)
{
    return type::null;
}

template <> constexpr type to_type(const std::chrono::system_clock::time_point&)
{
    return type::date;
}

template <> constexpr type to_type(const int&)
{
    return type::int32;
}

template <> constexpr type to_type(const long long&)
{
    return type::int64;
}

template <typename T> struct is_value : std::false_type {};

template <> struct is_value<std::int32_t> : std::true_type {};

template <> struct is_value<std::int64_t> : std::true_type {};

template <> struct is_value<double> : std::true_type {};

template <> struct is_value<bool> : std::true_type {};

template <> struct is_value<std::string> : std::true_type {};

template <> struct is_value<std::chrono::system_clock::time_point> : std::true_type {};

template <> struct is_value<std::nullptr_t> : std::true_type {};


template <typename T> struct is_value_array : std::false_type {};

template <typename T> struct is_value_array<std::initializer_list<T>> : is_value<T> {};

template <typename T> struct is_value_array<std::vector<T>> : is_value<T> {};

template <typename T, std::size_t N> struct is_value_array<std::array<T,N>> : is_value<T> {};

template <typename T, std::size_t N> struct is_value_array<T[N]> : is_value<T> {};


template <typename T> struct is_object_array : std::false_type {};

template <> struct is_object_array<std::initializer_list<object>> : std::true_type {};

template <> struct is_object_array<std::vector<object>> : std::true_type {};

template <std::size_t N> struct is_object_array<std::array<object,N>> : std::true_type {};

template <std::size_t N> struct is_object_array<object[N]> : std::true_type {};;

} // namespace xson

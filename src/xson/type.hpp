#pragma once

#include <ostream>
#include <cmath>
#include <string>
#include <chrono>
#include <array>
#include <vector>

namespace xson {

class object;

using byte_type    = std::uint8_t;
using number_type  = std::double_t;
using string_type  = std::string;
using object_type  = object;
using array_type   = object;
using boolean_type = bool;
using null_type    = std::nullptr_t;
using date_type    = std::chrono::system_clock::time_point;
using integer_type = std::int64_t;
using int32_type   = std::int32_t;

enum class type : xson::byte_type
{
    eod                   = '\x00',
    name                  = '\x13',
    index                 = '\x14',

// json types

    number                = '\x01',
    string                = '\x02',
    object                = '\x03',
    array                 = '\x04',
    boolean               = '\x08',
    null                  = '\x0A',

// + bson types
    date                  = '\x09',
//  timestamp             = '\x11',
    integer               = '\x12'
};

inline auto& operator << (std::ostream& os, type t)
{
    os << static_cast<int>(t);
    return os;
}


template <typename T> struct is_object : std::false_type {};

template <> struct is_object<object> : std::true_type {};

template <typename T> constexpr bool is_object_v = is_object<T>::value;


template <typename T> struct is_value : std::false_type {};

template <> struct is_value<xson::number_type> : std::true_type {};

template <> struct is_value<xson::string_type> : std::true_type {};

template <> struct is_value<xson::boolean_type> : std::true_type {};

template <> struct is_value<xson::null_type> : std::true_type {};

template <> struct is_value<xson::date_type> : std::true_type {};

template <> struct is_value<xson::integer_type> : std::true_type {};

template <> struct is_value<std::int32_t> : std::true_type {};

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

template <> struct is_object_array<object> : std::false_type {};

template <typename T> constexpr bool is_object_array_v = is_object_array<T>::value;

} // namespace xson

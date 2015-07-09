#pragma once

#include <cstdint>
#include <chrono>

namespace bson
{

struct document;
struct element;
struct array;
struct binary;
struct objectid;
struct regular_expression;
struct dbpointer;
struct javascript;
struct javascript_with_scope;
struct timestamp;

using byte_type = std::int8_t;

using element_type = element;

using double_type = double;                                // \x01
using string_type = std::string;                           // \x02
using document_type = document;                            // \x03
using array_type = array;                                  // \x04
using binary_type = binary;                                // \x05
using undefined_type = void;                               // \x06 — Deprecated
using objectid_type = objectid;                            // \x07
using boolean_type = bool;                                 // \x08
using date_type = std::chrono::system_clock::time_point;   // \x09
using null_type = std::nullptr_t;                          // \x0A
using regular_expression_type = regular_expression;        // \x0B
using dbpointer_type = dbpointer;                          // \x0C — Deprecated
using javascript_type = javascript;                        // \x0D
using deprecated_type = void;                              // \x0E
using javascript_with_scope_type = javascript_with_scope;  // \x0F
using int32_type = std::int32_t;                           // \x10
using timestamp_type = timestamp;                          // \x11
using int64_type = std::int64_t;                           // \x12

template <typename T> constexpr int32_type type(const T&)
{
    static_assert(std::is_void<T>::value, "This type is not supported yet");
    return 0x06;
};

template <> constexpr int32_type type(const double_type&)
{
    return 0x01;
};

template <> constexpr int32_type type(const string_type&)
{
    return 0x02;
};

template <> constexpr int32_type type(const document_type&)
{
    return 0x03;
};

template <> constexpr int32_type type(const array_type&)
{
    return 0x04;
};

template <> constexpr int32_type type(const boolean_type& b)
{
    return 0x08;
};

template <> constexpr int32_type type(const date_type& b)
{
    return 0x09;
};

template <> constexpr int32_type type(const null_type& b)
{
    return 0x0A;
};

template <> constexpr int32_type type(const int32_type&)
{
    return 0x10;
};

template <> constexpr int32_type type(const int64_type&)
{
    return 0x12;
};

} // namespace bson

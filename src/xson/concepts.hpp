#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <array>
#include <vector>

namespace xson {

class object;

using number_type    = std::double_t;
using string_type    = std::string;
using object_type    = object;
using array_type     = object;
using boolean_type   = bool;
using null_type      = std::nullptr_t;
using timestamp_type = std::chrono::system_clock::time_point;
using integer_type   = std::int64_t;
using int32_type     = std::int32_t;

template <typename T>
concept Number = std::same_as<std::remove_cvref_t<T>,number_type>;

template <typename T>
concept String = std::same_as<std::remove_cvref_t<T>,string_type>;

template <typename T>
concept Boolean = std::same_as<std::remove_cvref_t<T>,boolean_type>;

template <typename T>
concept Null = std::same_as<std::remove_cvref_t<T>,null_type>;

template <typename T>
concept Timestamp = requires(T t) {t.time_since_epoch();};

template <typename T>
concept Integer = (std::integral<std::remove_cvref_t<T>> and not std::same_as<std::remove_cvref_t<T>,boolean_type>);

template <typename T>
concept Value = (Number<T> or String<T> or Boolean<T> or Null<T> or Timestamp<T> or Integer<T>);

template <typename T>
concept Object = std::same_as<std::remove_cvref_t<T>,object>;

} // namespace xson

#pragma once

#include <memory>

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

struct decoder
{

decoder();

void decode(int32_type i);

void decode(int64_type i);

void decode(boolean_type b);

void decode(double_type d);

void decode(const cstring_type& str);

void decode(const element_type& e);

void decode(const array_type& a);

void decode(const document_type& d);

char* cbegin() const
{
    return head.get();
}

char* cend() const
{
    return head.get() + bytes;
}

int32_type size() const
{
    return bytes;
}

char* data()
{
    return head.get();
}

private:

void put(char b);

void put(const char* begin, const char* end);

std::unique_ptr<char> head;

char* current;

int32_type bytes;

};

} // namespace bson

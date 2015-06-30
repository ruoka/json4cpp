#pragma once

#include <memory>

namespace bson
{

struct document;
struct list;
struct element;
struct array;
struct binary;
struct objectid;

using byte_type = std::int8_t;
//using byte_type = char;
using int32_type = std::int32_t;
using int64_type = std::int64_t;
using double_type = double;
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

std::unique_ptr<char> head;

char* buffer = nullptr;

int32_type bytes = 0;;

decoder() : head{new char[1000]}
{
    buffer = head.get();
}

void decode(byte_type b)
{
    put(b & 0xFF);
}

void decode(int32_type i)
{
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
}

void decode(int64_type i)
{
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
    put((i >> 32) & 0xFF);
    put((i >> 40) & 0xFF);
    put((i >> 48) & 0xFF);
    put((i >> 56) & 0xFF);
}

void decode(double_type d)
{
    union{
        double_type d64;
        int64_type i64;
    } d2i;
    d2i.d64 = d;
    decode(d2i.i64);
}

void decode(const cstring_type& str)
{
    for(byte_type b : str)
        decode(b);
    decode(byte_type{'\x00'});
}

void decode(const decoder& d)
{
//    bytes += d.bytes; FIXME
//    std::copy(buffer, d.head.get(), d.head.get() + d.bytes); FIXME
}

char* data()
{
    return head.get();
}

private:

void put(char b)
{
    *buffer = b;
    ++buffer;
    ++bytes;
}

};

} // namespace bson

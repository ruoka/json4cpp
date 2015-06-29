#pragma once

#include <iostream>
#include "bson/model.hpp"

using namespace std;

namespace bson
{

char* buffer;

void put(char b)
{
    *buffer = b;
    ++buffer;
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
    decode('\x00');
}

} // namespace bson

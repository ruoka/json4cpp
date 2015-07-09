#pragma once

#include <vector>
#include "bson/type.hpp"

namespace bson
{

class decoder
{
public:

decoder();

void decode(byte_type b);

void decode(int32_type i);

void decode(int64_type i);

void decode(double_type d);

void decode(boolean_type b);

void decode(date_type n);

void decode(null_type n);

void decode(const string_type& str, bool csting = false);

void decode(const element_type& e);

void decode(const document_type& d);

void decode(const array_type& a);

int32_type size() const
{
    return buffer.size();
}

char* data()
{
    return buffer.data();
}

private:

using const_iterator = std::vector<char>::const_iterator;

void put(char b);

void put(const_iterator begin, const_iterator end);

const_iterator cbegin() const
{
    return buffer.cbegin();
}

const_iterator cend() const
{
    return buffer.end();
}

std::vector<char> buffer;

};

} // namespace bson

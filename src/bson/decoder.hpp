#pragma once

#include <vector>
#include "bson/type.hpp"

namespace bson
{

class decoder
{
public:

decoder();

void decode(int32_type i);

void decode(int64_type i);

void decode(boolean_type b);

void decode(double_type d);

void decode(const cstring_type& str);

void decode(const element_type& e);

void decode(const array_type& a);

void decode(const document_type& d);

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

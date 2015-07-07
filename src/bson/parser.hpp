#pragma once

#include <map>
#include <string>
#include <iosfwd>
#include <iostream>
#include "bson/type.hpp"

namespace bson
{

struct object
{
    object() :
    value{}, value_type{0x03}, objects{}
    {}

    object(std::istream& is) : object()
    {
        is >> std::skipws;
        parse_document(is,*this);
    }

    object(object&& c) :
    value{std::move(c.value)}, value_type{c.value_type}, objects{std::move(c.objects)}
    {}

    friend std::ostream& operator << (std::ostream&, const object&);

private:

    template <typename T>
    int32_type parse_value(std::istream& is, object& result);

    int32_type parse_string(std::istream& is, object& result);

    int32_type parse_document(std::istream& is, object& result);

    std::string value;

    int32_type value_type;

    std::map<std::string,object> objects;

    object(const object&) = delete;
    object& operator = (const object&) = delete;
    object& operator = (object&&) = delete;
};

template <typename T>
int32_type object::parse_value(std::istream& is, object& result)
{
    T value;
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
    std::clog << "val: "  << std::boolalpha << value << "; size: " << sizeof(T) << std::endl;
    result.value = std::to_string(value);
    result.value_type = bson::type(value);
    return is.gcount();
}

int32_type object::parse_string(std::istream& is, object& result)
{
    std::string value;
    std::getline(is, value, '\x00');
    std::clog << "val: " << value << std::endl;
    result.value = value;
    result.value_type = bson::type(value);
    return is.gcount();
}

int32_type object::parse_document(std::istream& is, object& result)
{
    int32_type length;
    is.read(reinterpret_cast<char*>(&length), sizeof(length));
    std::clog << "bytes: " << length << std::endl;

    int32_type bytes{length};

    while(bytes > 1)
    {
        int32_type type;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        bytes -= is.gcount();

        cstring_type name;
        std::getline(is, name, '\x00');
        bytes -= name.size();
        --bytes;
        std::clog << "key: " << name << std::endl;

        if(type == 0x01)
        {
            bytes -= parse_value<double>(is, result.objects[name]);
        }
        else if(type == 0x02)
        {
            bytes -= parse_string(is, result.objects[name]);
        }
        else if(type == 0x03 || type == 0x04)
        {
            bytes -= parse_document(is, result.objects[name]);
            result.objects[name].value_type = type;
        }
        else if(type == 0x08 || type == 0x09)
        {
            bytes -= parse_value<bool>(is, result.objects[name]);
        }
        else if(type == 0x10)
        {
            bytes -= parse_value<int>(is, result.objects[name]);
        }
        else if(type == 0x12)
        {
            bytes -= parse_value<long long>(is, result.objects[name]);
        }
        else
        {
            assert(false);
        }

        std::clog << bytes << std::endl;
    }

    is.ignore(1);
    bytes -= is.gcount();
    std::clog << bytes << std::endl;

    return length + 4;
}

object parse(std::istream& is)
{
    return object{is};
}

std::ostream& operator << (std::ostream& os, const object& c)
{
    if(!c.objects.empty() && c.value_type == 0x03)
    {
        os << '{';
        for(auto it = c.objects.cbegin(); it != c.objects.cend(); ++it)
        {
            if (it != c.objects.cbegin()) os << ',';
            os << '\"' << it->first << '\"' << ':' << std::boolalpha << it->second;
        }
        os << '}';
    }
    else if(!c.objects.empty() && c.value_type == 0x04)
    {
        os << '[';
        for(auto it = c.objects.cbegin(); it != c.objects.cend(); ++it)
        {
            if (it != c.objects.cbegin()) os << ',';
            os << std::boolalpha << it->second;
        }
        os << ']';
     }
    else if(!c.value.empty() && c.value_type == 0x02)
        os << '\"' << c.value << '\"';
    else if(!c.value.empty())
        os << std::boolalpha << c.value;
    else if (c.value_type == 0x03 || c.value_type == 0x04)
        os << "{}";
    else
        os << "null";

    return os;
}

} // namespace bson

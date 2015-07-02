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

private:

    void parse_string(std::istream& is, object& result);

    template <typename T>
    void parse_value(std::istream& is, object& result);

    void parse_document(std::istream& is, object& result)
    {
        int32_type bytes;
        is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
        std::clog << bytes << std::endl;

        while(bytes > 1)
        {
            int32_type type;
            is.read(reinterpret_cast<char*>(&type), sizeof(type));
            bytes -= is.gcount();

            cstring_type name;
            std::getline(is, name, '\x00');
            bytes -= name.size();
            --bytes;

            if(type == 0x01)
            {
                parse_value<double>(is, result.objects[name]);
            }
            else if(type == 0x02)
            {
                parse_string(is, result.objects[name]);
            }
            else if(type == 0x03 || type == 0x04)
            {
                parse_document(is, result.objects[name]);
                result.objects[name].value_type = type;
            }
            else if(type == 0x08 || type == 0x09)
            {
                parse_value<bool>(is, result.objects[name]);
            }
            else if(type == 0x10)
            {
                parse_value<int>(is, result.objects[name]);
            }
            else if(type == 0x12)
            {
                parse_value<long long>(is, result.objects[name]);
            }
            else
            {
                assert(false);
            }
            bytes -= is.gcount();
            std::clog << bytes << std::endl;
        }

        is.ignore(1);
        bytes -= is.gcount();
        std::clog << bytes << std::endl;
    }

    std::string value;

    int32_type value_type;

    std::map<std::string,object> objects;

    object(const object&) = delete;
    object& operator = (const object&) = delete;
    object& operator = (object&&) = delete;
};

template <typename T>
void object::parse_value(std::istream& is, object& result)
{
    T value;
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
    std::clog << std::boolalpha << value << " size " << sizeof(T) << std::endl;
    result.value = std::to_string(value);
    result.value_type = bson::type(value);
}

void object::parse_string(std::istream& is, object& result)
{
    std::string value;
    std::getline(is, value, '\x00');
    std::clog << value << std::endl;
    result.value = value;
    result.value_type = bson::type(value);
}

object parse(std::istream& is)
{
    return object{is};
}

} // namespace bson

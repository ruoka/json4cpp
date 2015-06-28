#pragma once

#include <map>
#include <string>
#include <iosfwd>
#include <iostream>

namespace json
{

enum class type
{
    string,
    number,
    object,
    array,
    boolean,
    null
};

struct collection
{
    collection& operator [] (const std::string& name)
    {
        return object[name];
    }

    collection& operator [] (std::size_t idx)
    {
        return object[std::to_string(idx)];
    }

    friend std::ostream& operator << (std::ostream& os, const collection& c)
    {
        if(!c.object.empty() && c.type == type::object)
        {
            os << '{';
            for(auto it = c.object.begin(); it != c.object.end(); ++it)
            {
                if (it != c.object.begin()) os << ',';
                os << '\"' << it->first << '\"' << ':' << it->second;
            }
            os << '}';
        }
        else if(!c.object.empty() && c.type == type::array)
        {
            os << '[';
            for(auto it = c.object.begin(); it != c.object.end(); ++it)
            {
                if (it != c.object.begin()) os << ',';
                os << it->second;
            }
            os << ']';
         }
        else if(!c.value.empty() && c.type == type::string)
            os << "\"" << c.value << "\"";
        else if(!c.value.empty())
            os << c.value;
        else
            os << "{}";

        return os;
    }

    std::string value;

    std::map<std::string,collection> object;

    json::type type = json::type::object;
};

void parse_document(std::istream& is, collection& result);

void parse_array(std::istream& is, collection& result)
{
    std::size_t idx{0};
    char next;
    is >> next; // [

    while(next != ']' && is)
    {
        std::string name{std::to_string(idx++)};
        std::string value;

        next = is.peek();

        if (next == '{')
        {
            parse_document(is, result.object[name]);
            is >> next; // , or ]
        }
        else if (next == '[')
        {
            parse_array(is, result.object[name]);
            is >> next; // , or ]
        }
        else if (next == '\"')
        {
            is >> next;
            getline(is, value, '\"'); // value"
            is >> next; // , or ]
            auto& o = result.object[name];
            o.type = type::string;
            o.value = value;
        }
        else
        {
            is >> next;
            while (next != ',' && next != ']')
            {
                value += next;
                is >> next;
            }
            auto& o = result.object[name];
            o.type = type::number;
            o.value = value;
        }

        result.type = type::array;
        std::clog << name << ":" << result.object[name] << std::endl;
    }
}

void parse_document(std::istream& is, collection& result)
{
    char next;
    is >> next; // {

    while(next != '}' && is)
    {
        std::string name, value;

        is >> next; // "
        getline(is, name, '\"'); // name"
        is >> next; // :

        next = is.peek();

        if (next == '{')
        {
            parse_document(is, result.object[name]);
            is >> next; // , or }
        }
        else if (next == '[')
        {
            parse_array(is, result.object[name]);
            is >> next; // , or }
        }
        else if (next == '\"')
        {
            result.type = type::string;
            is >> next; // "
            getline(is, value, '\"'); // value"
            is >> next; // , or }
            auto& o = result.object[name];
            o.type = type::string;
            o.value = value;
        }
        else
        {
            is >> next;
            while (next != ',' && next != '}')
            {
                value += next;
                is >> next;
            }
            auto& o = result.object[name];
            o.type = type::number;
            o.value = value;
        }

        result.type = type::object;
        std::clog << name << ":" << result.object[name] << std::endl;
    }
}

collection parse(std::istream& is)
{
    collection result;
    is >> std::skipws;
    parse_document(is, result);
    return result;
}

} // namespace json

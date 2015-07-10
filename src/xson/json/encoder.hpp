#pragma once

#include "xson/object.hpp"

namespace xson {
namespace json {

using namespace std::literals::string_literals;

std::istream& operator >> (std::istream& os, object& obj);

class encoder
{
public:

    encoder() = delete;

    static void encode(std::istream& is, object& obj)
    {
        encode_document(is, obj);
    }

private:

    static void encode_string(std::istream& is, object& obj)
    {
        char next;
        std::string value;
        is >> next; // "
        getline(is, value, '\"'); // value"
        obj.m_value = value;
        obj.m_type = object::type::string;
    }

    static void encode_value(std::istream& is, object& obj)
    {
        char next;
        std::string value;
        is >> next;
        while (next != ',' && next != '}' && next != ']')
        {
            value += next;
            is >> next;
        }
        is.putback(next); // , }, or ] do not belong to values
        obj.m_value = value;
        if(value == "true"s || value == "false"s)
            obj.m_type = object::type::boolean;
        else if(value == "null"s)
            obj.m_type = object::type::null;
        else
            obj.m_type = object::type::number;
    }

    static void encode_array(std::istream& is, object& obj)
    {
        std::size_t idx{0};
        char next;
        is >> next; // [

        while(next != ']' && is)
        {
            std::string name{std::to_string(idx++)};

            is >> std::ws;
            next = is.peek();

            if (next == '{')
            {
                encode_document(is, obj.m_objects[name]);
                is >> next; // , or ]
            }
            else if (next == '[')
            {
                encode_array(is, obj.m_objects[name]);
                is >> next; // , or ]
            }
            else if (next == '\"')
            {
                encode_string(is, obj.m_objects[name]);
                is >> next; // , or ]
            }
            else
            {
                encode_value(is, obj.m_objects[name]);
                is >> next; // , }, or ]
            }
        }
        obj.m_type = object::type::array;
    }

    static void encode_document(std::istream& is, object& obj)
    {
        char next;
        is >> next; // {

        while(next != '}' && is)
        {
            std::string name, value;
            is >> std::ws >> next; // "
            getline(is, name, '\"'); // name"
            is >> next; // :

            is >> std::ws;
            next = is.peek();

            if(next == '{')
            {
                encode_document(is, obj.m_objects[name]);
                is >> next; // , or }
            }
            else if(next == '[')
            {
                encode_array(is, obj.m_objects[name]);
                is >> next; // , or }
            }
            else if(next == '\"')
            {
                encode_string(is, obj.m_objects[name]);
                is >> next; // , or ]
            }
            else
            {
                encode_value(is, obj.m_objects[name]);
                is >> next; // , }, or ]
            }
        }
        obj.m_type = object::type::object;
    }
};

inline std::istream& operator >> (std::istream& is, object& obj)
{
    encoder::encode(is, obj);
    return is;
}

} // namespace json
} // namespace xson

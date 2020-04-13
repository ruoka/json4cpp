#pragma once

#include "xson/object.hpp"

namespace xson::json::_1 {

using namespace std::string_literals;

class decoder
{
public:

    decoder(std::istream& is) : m_is{is}
    {}

    void decode(object& obj)
    {
        auto next = ' ';
        m_is >> std::ws;
        next = m_is.peek();
        if(next == '{')
            decode_object(obj);
        else if(next == '[')
            decode_array(obj);
        else
            throw std::invalid_argument{"Invalid JSON object "s + next};
    }

private:

    void decode_string(object& obj)
    {
        auto next = ' ';
        auto value = ""s;
        m_is >> next;                 // "
        getline(m_is, value, '\"'); // value"
        obj = value;
    }

    void decode_value(object& obj)
    {
        auto next = ' ';
        auto value = ""s;
        m_is >> next;
        while(next != ',' && next != '}' && next != ']')
        {
            value += next;
            m_is >> next;
        }
        m_is.putback(next); // , }, or ] do not belong to values

        try
        {
            if(value.find('.') != value.npos)
                obj = stod(value);
            else if(value == "true")
                obj = true;
            else if(value == "false")
                obj = false;
            else if(value == "null")
                obj = nullptr;
            else
                obj = stoll(value);
        }
        catch(const std::invalid_argument&)
        {
            obj = value;
        }
    }

    void decode_array(object& parent)
    {
        auto idx = size_t{0};
        auto next = ' ';
        m_is >> next;                         // [
        while(next != ']' && m_is)
        {
            m_is >> std::ws;
            next = m_is.peek();               // ], {, [, " or empty
            if(next != ']')
            {
                object& child = parent[idx++];
                m_is >> std::ws;
                next = m_is.peek();           // {, [, " or empty
                if (next == '{')
                    decode_object(child);
                else if (next == '[')
                    decode_array(child);
                else if (next == '\"')
                    decode_string(child);
                else
                    decode_value(child);
            }
            m_is >> next;                     // , or ]
        }
        parent.type(type::array);
    }

    void decode_object(object& parent)
    {
        auto next = ' ';
        m_is >> next;                         // {
        while(next != '}' && m_is)
        {
            m_is >> std::ws;
            next = m_is.peek();               // } or "
            if(next != '}')
            {
                auto name = ""s;
                m_is >> next;                 // "
                getline(m_is, name, '\"');  // name"
                m_is >> next;                 // :
                object& child = parent[name];
                m_is >> std::ws;
                next = m_is.peek();           // {, [, " or empty
                if(next == '{')
                    decode_object(child);
                else if(next == '[')
                    decode_array(child);
                else if(next == '\"')
                    decode_string(child);
                else
                    decode_value(child);
            }
            m_is >> next;                     // , or }
        }
        parent.type(type::object);
    }

    std::istream& m_is;
};

inline auto& operator >> (std::istream& is, xson::object& obj)
{
    xson::json::_1::decoder{is}.decode(obj);
    return is;
}

} // namespace xson::json::__1

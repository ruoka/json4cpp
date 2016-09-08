#pragma once

#include "xson/object.hpp"

namespace xson::json {

using namespace std::string_literals;

std::istream& operator >> (std::istream& os, object& ob);

class decoder
{
public:

    decoder(std::istream& is) : m_is{is}
    {}

    void decode(object& obj)
    {
        auto next = u8' ';
        m_is >> std::ws;
        next = m_is.peek();
        if(next == u8'{')
            decode_object(obj);
        else if(next == u8'[')
            decode_array(obj);
        else
            throw std::invalid_argument{"Invalid JSON object "s + next};
    }

private:

    void decode_string(object& obj)
    {
        auto next = u8' ';
        auto value = ""s;
        m_is >> next;                 // "
        getline(m_is, value, u8'\"'); // value"
        obj = value;
    }

    void decode_value(object& obj)
    {
        auto next = u8' ';
        auto value = ""s;
        m_is >> next;
        while (next != u8',' && next != u8'}' && next != u8']')
        {
            value += next;
            m_is >> next;
        }
        m_is.putback(next); // , }, or ] do not belong to values

        try
        {
            if(value.find(u8'.') != std::string::npos)
                obj = stod(value);
            else if(value == "true")
                obj = true;
            else if(value == "false")
                obj = false;
            else if(value == "null")
                obj= nullptr;
            else
                obj = stoi(value);
        }
        catch(const std::out_of_range&)
        {
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
        auto next = u8' ';
        m_is >> next;                         // [
        while(next != u8']' && m_is)
        {
            m_is >> std::ws;
            next = m_is.peek();               // ], {, [, " or empty
            if(next != u8']')
            {
                object& child = parent[idx++];
                m_is >> std::ws;
                next = m_is.peek();           // {, [, " or empty
                if (next == u8'{')
                    decode_object(child);
                else if (next == u8'[')
                    decode_array(child);
                else if (next == u8'\"')
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
        auto next = u8' ';
        m_is >> next;                         // {
        while(next != u8'}' && m_is)
        {
            m_is >> std::ws;
            next = m_is.peek();               // } or "
            if(next != u8'}')
            {
                auto name = ""s;
                m_is >> next;                 // "
                getline(m_is, name, u8'\"');  // name"
                m_is >> next;                 // :
                object& child = parent[name];
                m_is >> std::ws;
                next = m_is.peek();           // {, [, " or empty
                if(next == u8'{')
                    decode_object(child);
                else if(next == u8'[')
                    decode_array(child);
                else if(next == u8'\"')
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

inline std::istream& operator >> (std::istream& is, object& obj)
{
    decoder{is}.decode(obj);
    return is;
}

} // namespace xson::json

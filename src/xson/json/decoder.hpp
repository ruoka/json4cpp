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

    void decode(object& ob)
    {
        decode_document(ob);
    }

private:

    void decode_string(object& obj)
    {
        auto next = u8' ';
        auto value = ""s;
        m_is >> next;                 // "
        getline(m_is, value, u8'\"'); // value"
        obj.value(value);
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
                obj.value(std::stod(value));
            else if(value == "true")
                obj.value(true);
            else if(value == "false")
                obj.value(false);
            else if(value == "null")
                obj.value(nullptr);
            else
                obj.value(std::stoi(value));
        }
        catch(const std::out_of_range&)
        {
            obj.value(std::stoll(value));
        }
        catch(const std::invalid_argument&)
        {
            obj.value(value);
        }
    }

    void decode_array(object& parent)
    {
        auto idx = std::size_t{0};
        auto next = u8' ';
        m_is >> next; // [

        while(next != u8']' && m_is)
        {
            const auto name = std::to_string(idx++);
            auto& child = parent[name];
            m_is >> std::ws;
            next = m_is.peek();

            if (next == u8'{')
            {
                decode_document(child);
                m_is >> next; // , or ]
            }
            else if (next == u8'[')
            {
                decode_array(child);
                m_is >> next; // , or ]
            }
            else if (next == u8'\"')
            {
                decode_string(child);
                m_is >> next; // , or ]
            }
            else
            {
                decode_value(child);
                m_is >> next; // , or ]
            }
        }
        parent.type(type::array);
    }

    void decode_document(object& parent)
    {
        auto next = u8' ';
        m_is >> next; // {

        while(next != u8'}' && m_is)
        {
            auto name = ""s;
            m_is >> std::ws >> next;     // "
            getline(m_is, name, u8'\"'); // name"
            m_is >> next;                // :

            auto& child = parent[name];

            m_is >> std::ws;
            next = m_is.peek();

            if(next == u8'{')
            {
                decode_document(child);
                m_is >> next; // , or }
            }
            else if(next == u8'[')
            {
                decode_array(child);
                m_is >> next; // , or }
            }
            else if(next == u8'\"')
            {
                decode_string(child);
                m_is >> next; // , or }
            }
            else
            {
                decode_value(child);
                m_is >> next; // , or }
            }
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

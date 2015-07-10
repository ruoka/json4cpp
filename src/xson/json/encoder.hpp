#pragma once

#include "xson/object.hpp"

namespace xson {
namespace json {

using namespace std::literals::string_literals;

std::istream& operator >> (std::istream& os, object& ob);

class encoder
{
public:

    encoder(std::istream& is) : m_is{is}
    {}

    void encode(object& ob)
    {
        encode_document(ob);
    }

private:

    void encode_string(object& ob)
    {
        char next;
        std::string value;
        m_is >> next; // "
        getline(m_is, value, '\"'); // value"
        ob.value(value);
    }

    void encode_value(object& ob)
    {
        char next;
        std::string value;
        m_is >> next;
        while (next != ',' && next != '}' && next != ']')
        {
            value += next;
            m_is >> next;
        }
        m_is.putback(next); // , }, or ] do not belong to values
        ob.value(value); // FIXME?
    }

    void encode_array(object& parent)
    {
        std::size_t idx{0};
        char next;
        m_is >> next; // [

        while(next != ']' && m_is)
        {
            const std::string name{std::to_string(idx++)};
            auto& child = parent[name];

            m_is >> std::ws;
            next = m_is.peek();

            if (next == '{')
            {
                encode_document(child);
                m_is >> next; // , or ]
            }
            else if (next == '[')
            {
                encode_array(child);
                m_is >> next; // , or ]
            }
            else if (next == '\"')
            {
                encode_string(child);
                m_is >> next; // , or ]
            }
            else
            {
                encode_value(child);
                m_is >> next; // , }, or ]
            }
        }
        parent.value(object::type::array);
    }

    void encode_document(object& parent)
    {
        char next;
        m_is >> next; // {

        while(next != '}' && m_is)
        {
            std::string name;
            m_is >> std::ws >> next; // "
            getline(m_is, name, '\"'); // name"
            m_is >> next; // :

            auto& child = parent[name];

            m_is >> std::ws;
            next = m_is.peek();

            if(next == '{')
            {
                encode_document(child);
                m_is >> next; // , or }
            }
            else if(next == '[')
            {
                encode_array(child);
                m_is >> next; // , or }
            }
            else if(next == '\"')
            {
                encode_string(child);
                m_is >> next; // , or ]
            }
            else
            {
                encode_value(child);
                m_is >> next; // , }, or ]
            }
        }
        parent.value(object::type::object);
    }

    std::istream& m_is;
};

inline std::istream& operator >> (std::istream& is, object& ob)
{
    encoder{is}.encode(ob);
    return is;
}

} // namespace json
} // namespace xson

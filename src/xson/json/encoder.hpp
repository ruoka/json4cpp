#pragma once

#include "xson/object.hpp"

namespace xson::json {

inline std::ostream& operator << (std::ostream& os, const object::value& v)
{
    if(holds_alternative<number_type>(v))
        os << std::get<number_type>(v);
    else if(holds_alternative<string_type>(v))
        os << '"' << std::get<string_type>(v) << '"';
    else if(holds_alternative<boolean_type>(v))
        os << std::boolalpha << std::get<boolean_type>(v);
    else if(holds_alternative<timestamp_type>(v))
        os << '"' << to_string(std::get<timestamp_type>(v)) << '"';
    else if(holds_alternative<monostate>(v))
        os << "null";
    else if(holds_alternative<integer_type>(v))
        os << std::get<integer_type>(v);
    return os;
}

class encoder
{
public:

    encoder(std::streamsize indent = 2) : m_pretty{indent}
    {}

    void encode(std::ostream& os, const object& o)
    {
        if(o.is_object())
        {
            const auto& container = o.get<object::map>();
            os << m_pretty('{');
            for(auto i = 0; const auto& [name,value] : container)
            {
                os << (i++ == 0 ? m_pretty() : m_pretty(','));
                os << '\"' << name << '\"' << m_pretty(':');
                encode(os,value);
            }
            os << m_pretty('}', container.empty());
        }
        else if(o.is_array())
        {
            const auto& container = o.get<object::array>();
            os << m_pretty('[');
            for(auto i = 0; const auto& value : container)
            {
                os << (i++ == 0 ? m_pretty() : m_pretty(','));
                encode(os,value);
            }
            os << m_pretty(']', container.empty());
        }
        else
            os << o.get<object::value>();
    }

private:

    class prettyprint
    {
    public:

        prettyprint(std::streamsize indent) : m_indent{indent}, m_level{0}
        {}

        std::string operator () ()
        {
            if(!m_indent)
                return ""s;
            else
                return "\n"s + m_pretty;
        }

        std::string operator () (char c, bool empty = false)
        {
            if(!m_indent)
                return std::string{c};

            if(c == ':')
                return " : "s;

            if(c == ',')
                return  ",\n"s + m_pretty;

            if(c == '{' || c == '[')
            {
                ++m_level;
                m_pretty = std::string(m_level * m_indent, ' ');
                return std::string{c};
            }

            if(c == '}' || c == ']')
            {
                --m_level;
                m_pretty = std::string(m_level * m_indent, ' ');
                if(!empty)
                    return  "\n"s + m_pretty + c;
                else
                    return std::string{c};
            }

            return std::string{c};
        }

        friend std::ostream& operator << (std::ostream& os, const prettyprint& p)
        {
            os << p.m_pretty;
            return os;
        }

    private:

        std::streamsize m_indent;

        std::streamsize m_level;

        std::string m_pretty;
    };

    prettyprint m_pretty;
};

} // namespace xson::json

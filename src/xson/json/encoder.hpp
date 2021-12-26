#pragma once

#include "xson/object.hpp"
#include "std/extension.hpp"

namespace xson::json {

inline std::ostream& operator << (std::ostream& os, const object::value& v)
{
    if(holds_alternative<number_type>(v))
        os << std::get<number_type>(v);
    else if(holds_alternative<string_type>(v))
        os << '"' << std::get<string_type>(v) << '"';
    else if(holds_alternative<boolean_type>(v))
        os << std::boolalpha << std::get<boolean_type>(v);
    else if(holds_alternative<date_type>(v))
        os << '"' << ext::to_string(std::get<date_type>(v)) << '"';
    else if(holds_alternative<monostate>(v))
        os << "null";
    else if(holds_alternative<integer_type>(v))
        os << std::get<integer_type>(v);
    return os;
}

class encoder
{
public:

    encoder(std::ostream& os, std::streamsize indent = 2) : m_pretty{indent}, m_os{os}
    {}

    void encode(const object& o)
    {
        if(o.type() == type::object)
        {
            const auto& container = o.get<object::map>();
            m_os << m_pretty('{');
            for(auto i = 0; const auto& [name,value] : container)
            {
                m_os << (i++ == 0 ? m_pretty() : m_pretty(','));
                m_os << '\"' << name << '\"' << m_pretty(':');
                encode(value);
            }
            m_os << m_pretty('}', container.empty());
        }
        else if(o.type() == type::array)
        {
            const auto& container = o.get<object::array>();
            m_os << m_pretty('[');
            for(auto i = 0; const auto& value : container)
            {
                m_os << (i++ == 0 ? m_pretty() : m_pretty(','));
                encode(value);
            }
            m_os << m_pretty(']', container.empty());
        }
        else
            m_os << o.get<object::value>();
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

    std::ostream& m_os;
};

} // namespace xson::json

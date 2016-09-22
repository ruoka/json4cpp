#pragma once

#include "xson/object.hpp"

namespace xson::json {

inline std::ostream& operator << (std::ostream& os, const value& val)
{
    if(holds_alternative<number_type>(val))
        os << get<number_type>(val);
    else if(holds_alternative<string_type>(val))
        os << '"' << get<string_type>(val) << '"';
    else if(holds_alternative<boolean_type>(val))
        os << std::boolalpha << get<boolean_type>(val);
    else if(holds_alternative<date_type>(val))
        os << '"' << to_string(get<date_type>(val)) << '"';
    else if(holds_alternative<null_type>(val))
        os << "null";
    else if(holds_alternative<int32_type>(val))
        os << get<int32_type>(val);
    else if(holds_alternative<int64_type>(val))
        os << get<int64_type>(val);
    return os;
}

class encoder
{
public:

    encoder(std::ostream& os, std::streamsize indent = 2) : m_pretty{indent}, m_os{os}
    {}

    void encode(const object& obj)
    {
        if(obj.type() == type::object)
        {
            m_os << m_pretty('{');
            for(auto it = obj.cbegin(); it != obj.cend(); ++it)
            {
                if(it == obj.cbegin()) m_os << m_pretty(); else m_os << m_pretty(',');
                m_os << '\"' << it->first << '\"' << m_pretty(':');
                encode(it->second);
            }
            m_os << m_pretty('}', obj.empty());
        }
        else if(obj.type() == type::array)
        {
            m_os << m_pretty('[');
            for(auto it = obj.cbegin(); it != obj.cend(); ++it)
            {
                if(it == obj.cbegin()) m_os << m_pretty(); else m_os << m_pretty(',');
                encode(it->second);
            }
            m_os << m_pretty(']', obj.empty());
        }
        else
            m_os << obj.value();
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

inline auto& operator << (std::ostream& os, const xson::object& obj)
{
    const auto indent = os.width();
    auto e = json::encoder{os, indent};
    os.width(0);
    e.encode(obj);
    os.width(indent);
    return os;
}

} // namespace xson::json

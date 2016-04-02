#pragma once

#include "xson/object.hpp"

namespace xson::json {

class encoder
{
public:

    encoder(std::ostream& os, std::streamsize indent = 2) : m_pretty{indent}, m_os{os}
    {}

    void encode(const object& ob)
    {
        if(ob.type() == type::object)
        {
            m_os << m_pretty('{');
            for(auto it = ob.cbegin(); it != ob.cend(); ++it)
            {
                if (it != ob.cbegin()) m_os << ',';
                m_os << m_pretty << '\"' << it->first << '\"' << m_pretty(':');
                encode(it->second);
            }
            m_os << m_pretty('}');
        }
        else if(ob.type() == type::array)
        {
            m_os << m_pretty('[');
            for(auto it = ob.cbegin(); it != ob.cend(); ++it)
            {
                if (it != ob.cbegin()) m_os << ',';
                m_os << m_pretty;
                encode(it->second);
            }
            m_os << m_pretty(']');
        }
        else if(ob.value().empty())
            m_os << "null";
        else if(ob.type() == type::string || ob.type() == type::date)
            m_os << '\"' << ob.value() << '\"';
        else
            m_os << ob.value();
    }

private:

    class prettyprint
    {
    public:

        prettyprint(std::streamsize indent) : m_indent{indent}, m_level{0}
        {}

        std::string operator () (char c)
        {
            if(!m_indent)
                return std::string{c};

            if(c == ':')
                return std::string{" : "};

            if(c == '{' || c == '[')
                ++m_level;

            if(c == '}' || c == ']')
                --m_level;

            m_pretty = "\n"s + std::string(m_level * m_indent, ' ');

            if (c == '}' || c == ']')
                return m_pretty + std::string{c};
            else
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

inline std::ostream& operator << (std::ostream& os, const object& ob)
{
    const auto indent = os.width();
    auto e = encoder{os, indent};
    os.width(0);
    e.encode(ob);
    os.width(indent);
    return os;
}

} // namespace xson::json

#pragma once

#include "xson/object.hpp"

namespace xson::json {

std::ostream& operator << (std::ostream& os, const object& ob);

class encoder
{
public:

    encoder(std::ostream& os) : m_os{os}
    {}

    void encode(const object& ob)
    {
        if(ob.type() == type::object)
        {
            m_os << '{';
            for(auto it = ob.cbegin(); it != ob.cend(); ++it)
            {
                if (it != ob.cbegin()) m_os << ',';
                m_os << '\"' << it->first << '\"' << ':' << it->second;
            }
            m_os << '}';
        }
        else if(ob.type() == type::array)
        {
            m_os << '[';
            for(auto it = ob.cbegin(); it != ob.cend(); ++it)
            {
                if (it != ob.cbegin()) m_os << ',';
                m_os << it->second;
            }
            m_os << ']';
        }
        else if(ob.value().empty())
            m_os << "null";
        else if(ob.type() == type::string || ob.type() == type::date)
            m_os << '\"' << ob.value() << '\"';
        else
            m_os << ob.value();
    }

private:

    std::ostream& m_os;
};

inline std::ostream& operator << (std::ostream& os, const object& ob)
{
    encoder{os}.encode(ob);
    return os;
}

} // namespace xson::json

#pragma once

#include "xson/object.hpp"

namespace xson {
namespace json {

std::ostream& operator << (std::ostream& os, const object& obj);

class decoder
{
public:

    static void decode(std::ostream& os, const object& obj)
    {
        decode_document(os, obj);
    }

private:

    static void decode_document(std::ostream& os, const object& obj)
    {
        if(!obj.m_objects.empty() && obj.m_type == object::type::object)
        {
            os << '{';
            for(auto it = obj.m_objects.cbegin(); it != obj.m_objects.cend(); ++it)
            {
                if (it != obj.m_objects.cbegin()) os << ',';
                os << '\"' << it->first << '\"' << ':' << it->second;
            }
            os << '}';
        }
        else if(!obj.m_objects.empty() && obj.m_type == object::type::array)
        {
            os << '[';
            for(auto it = obj.m_objects.cbegin(); it != obj.m_objects.cend(); ++it)
            {
                if (it != obj.m_objects.cbegin()) os << ',';
                os << it->second;
            }
            os << ']';
        }
        else if(!obj.m_value.empty() && obj.m_type == object::type::string)
        os << '\"' << obj.m_value << '\"';
        else if(!obj.m_value.empty())
        os << obj.m_value;
        else if (obj.m_type == object::type::object || obj.m_type == object::type::array)
        os << "{}";
        else
        os << "null";
    }

};

inline std::ostream& operator << (std::ostream& os, const object& obj)
{
    decoder::decode(os, obj);
    return os;
}

} // namespace json
} // namespace xson

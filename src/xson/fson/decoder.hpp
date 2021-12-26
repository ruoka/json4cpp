#pragma once
#include <cassert>
#include <type_traits>
#include "gsl/not_null.hpp"
#include "xson/fast/decoder.hpp"
#include "xson/builder.hpp"

namespace xson::fson {

class decoder : public fast::decoder
{
public:

    decoder(std::istream& is, gsl::not_null<observer*> o) : fast::decoder{is}, m_observer{o}
    {}

    using fast::decoder::decode;

    void decode()
    {
        auto parent = std::stack<type>{};

        while(m_is)
        {
            auto type = xson::type{};
            decode(type);

            xson::string_type name;
            xson::integer_type idx;
            xson::number_type   d;
            xson::string_type str;
            xson::date_type    dt;
            xson::boolean_type  b;
            xson::integer_type  i;

            switch(type)
            {
                case type::object: // x03
                    parent.push(type::object);
                    m_observer->start_object();
                    break;

                case type::name:
                    decode(name);
                    m_observer->name(name);
                    break;

                case type::array:  // x04
                    parent.push(type::array);
                    m_observer->start_array();
                    break;

                case type::index:
                    decode(idx);
                    m_observer->index(idx);
                    break;

                case type::number: // x01
                    decode(d);
                    m_observer->value(d);
                    break;

                case type::string: // x02
                    decode(str);
                    m_observer->value(str);
                    break;

                case type::boolean: // x08
                    decode(b);
                    m_observer->value(b);
                    break;

                case type::null:    // x0A
                    m_observer->value(nullptr);
                    break;

                case type::date:    // x09
                    decode(dt);
                    m_observer->value(dt);
                    break;

                case type::integer: // x12
                    decode(i);
                    m_observer->value(i);
                    break;

                case type::eod:     // x00
                    if(parent.top() == type::object) m_observer->end_object();
                    if(parent.top() == type::array) m_observer->end_array();
                    parent.pop();
                    break;

                default:
                    assert(false);
                    break;
            }

            if(parent.empty()) return;
        }
    }

private:

    gsl::not_null<observer*> m_observer;

};

} // namespace xson::fson

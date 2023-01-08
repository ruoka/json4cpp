#pragma once
#include <stack>
#include <cassert>
#include "xson/concepts.hpp"
#include "xson/fast/decoder.hpp"
#include "xson/fson/types.hpp"

namespace xson::fson {

template<typename Builder>
class decoder : public fast::decoder
{
public:

    decoder(std::istream& is, Builder& b) : fast::decoder{is}, m_builder{b}
    {}

    using fast::decoder::decode;

    void decode()
    {
        auto parent = std::stack<fson::type>{};

        while(m_is)
        {
            auto type = xson::fson::type{};
            decode(type);

            xson::string_type name;
            xson::number_type d;
            xson::string_type str;
            xson::timestamp_type dt;
            xson::boolean_type b;
            xson::integer_type i;

            switch(type)
            {
                case type::object: // x03
                    parent.push(type::object);
                    m_builder.start_object();
                    break;

                case type::name:
                    decode(name);
                    m_builder.name(name);
                    break;

                case type::array:  // x04
                    parent.push(type::array);
                    m_builder.start_array();
                    break;

                case type::number: // x01
                    decode(d);
                    m_builder.value(d);
                    break;

                case type::string: // x02
                    decode(str);
                    m_builder.value(str);
                    break;

                case type::boolean: // x08
                    decode(b);
                    m_builder.value(b);
                    break;

                case type::null:    // x0A
                    m_builder.value(nullptr);
                    break;

                case type::timestamp:    // x09
                    decode(dt);
                    m_builder.value(dt);
                    break;

                case type::integer: // x12
                    decode(i);
                    m_builder.value(i);
                    break;

                case type::end:     // x00
                    if(parent.top() == type::object) m_builder.end_object();
                    if(parent.top() == type::array) m_builder.end_array();
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

    Builder& m_builder;

};

} // namespace xson::fson

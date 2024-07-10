#pragma once
#include <stack>
#include "xson/builder.hpp"

namespace xson::json {

template<typename Builder>
class decoder
{
public:

    decoder(Builder& b) : m_builder{b}
    {}

    void decode(std::istream& is)
    {
        m_state_machine.push(&decoder::document);
        while(is)
        {
            m_state_machine.top()(*this,is.get());
            if(m_state_machine.empty()) break;
        }
        if(!m_state_machine.empty())
            throw std::runtime_error{"Invalid JSON object"s};
    }

    void decode(std::string_view sv)
    {
        m_state_machine.push(&decoder::document);
        for(auto c : sv)
        {
            m_state_machine.top()(*this,c);
            if(m_state_machine.empty()) break;
        }
        if(!m_state_machine.empty())
            throw std::runtime_error{"Invalid JSON object"s};
    }

private:

    template <bool Boolean>
    void constant(char c)
    {
        m_builder.value(Boolean);
        m_state_machine.pop();
        m_state_machine.top()(*this,c);
    }

    void null(char c)
    {
        m_builder.value(nullptr);
        m_state_machine.pop();
        m_state_machine.top()(*this,c);
    }

    template <char Character>
    void is(char c)
    {
        if(c == Character)
            m_state_machine.pop();
        else
            throw std::runtime_error{"failed: "s + c + " is not " + Character};
    }

    void string(char c)
    {
        if(c != '\"')
            m_string += c;
        else
        {
            m_builder.value(std::move(m_string));
            m_state_machine.pop();
            m_string = ""s;
        }
    }

    template<int Sign>
    void number(char c)
    {
        if(std::isdigit(c))
        {
            m_integer *= 10;
            m_integer += (c-'0');
        }
        else if(c == '.')
        {
            m_number = m_integer;
            m_integer = 0;
            m_state_machine.pop();
            m_state_machine.push(&decoder::fractions<Sign>);
        }
        else
        {
            m_builder.value(Sign * m_integer);
            m_integer = 0;
            m_state_machine.pop();
            m_state_machine.top()(*this,c);
        }
    }

    template<int Sign>
    void fractions(char c)
    {
        if(isdigit(c))
        {
            m_place *= 10;
            m_number += (static_cast<xson::number_type>(c-'0')/m_place);
        }
        else
        {
            m_builder.value(Sign * m_number);
            m_number = 0;
            m_place = 1;
            m_state_machine.pop();
            m_state_machine.top()(*this,c);
        }
    }

    void positive(char c) {return number<1>(c);}

    void negative(char c) {return number<-1>(c);}

    void value(char c)
    {
        if(c == '\"')
        {
            m_state_machine.push(&decoder::string);
        }
        else if(isdigit(c))
        {
            m_state_machine.push(&decoder::positive);
            m_state_machine.top()(*this,c);
        }
        else if(c == '-')
        {
            m_state_machine.push(&decoder::negative);
        }
        else if(c == 't')
        {
            m_state_machine.push(&decoder::constant<true>);
            m_state_machine.push(&decoder::is<'e'>);
            m_state_machine.push(&decoder::is<'u'>);
            m_state_machine.push(&decoder::is<'r'>);
        }
        else if(c == 'f')
        {
            m_state_machine.push(&decoder::constant<false>);
            m_state_machine.push(&decoder::is<'e'>);
            m_state_machine.push(&decoder::is<'s'>);
            m_state_machine.push(&decoder::is<'l'>);
            m_state_machine.push(&decoder::is<'a'>);
        }
        else if(c == 'n')
        {
            m_state_machine.push(&decoder::null);
            m_state_machine.push(&decoder::is<'l'>);
            m_state_machine.push(&decoder::is<'l'>);
            m_state_machine.push(&decoder::is<'u'>);
        }
        else if(c == '{' || c == '[')
        {
            m_state_machine.push(&decoder::document);
            m_state_machine.top()(*this,c);
        }
        else if (c == ',' || c == '}' || c == ']')
        {
            m_state_machine.pop();
            m_state_machine.top()(*this,c);
        }
        else if(!isws(c))
            throw std::runtime_error{"value failed: "s + c};
    }

    void comma(char c)
    {
        if(c == ',')
        {
            m_state_machine.pop();
        }
        else if(c == '}' || c == ']')
        {
            m_state_machine.pop();
            m_state_machine.top()(*this,c);
        }
        else if(!isws(c))
            throw std::runtime_error{"comma failed: "s + c};
    }

    void colon(char c)
    {
        if(c == ':')
            m_state_machine.pop();
        else if(!isws(c))
            throw std::runtime_error{"colon failed: "s + c};
    }

    void name(char c)
    {
        if(c != '\"')
            m_string += c;
        else
        {
            m_builder.name(m_string);
            m_string = ""s;
            m_state_machine.pop();
        }
    }

    void object(char c)
    {
        if(c == '}')
        {
            m_state_machine.pop();
            m_state_machine.top()(*this,c);
        }
        else if(c == '\"')
        {
            m_state_machine.push(&decoder::comma);
            m_state_machine.push(&decoder::value);
            m_state_machine.push(&decoder::colon);
            m_state_machine.push(&decoder::name);
        }
        else if(!isws(c))
            throw std::runtime_error{"object failed: "s + c};
    }

    struct array
    {
        void operator()(decoder& p, char c)
        {
            if(c == ']')
            {
                p.m_state_machine.pop();
                p.m_state_machine.top()(p,c);
            }
            else if (c != ',' && c != '}')
            {
                p.m_state_machine.push(&decoder::comma);
                p.m_state_machine.push(&decoder::value);
                p.m_state_machine.top()(p,c);
            }
            else if(!isws(c))
                throw std::runtime_error{"array failed: "s + c};

        }
    };

    void document(char c)
    {
        if(c == '{')
        {
            m_builder.start_object();
            m_state_machine.push(&decoder::object);
        }
        else if(c == '}')
        {
            m_builder.end_object();
            m_state_machine.pop();
        }
        else if(c == '[')
        {
            m_builder.start_array();
            m_state_machine.push(array{});
        }
        else if(c == ']')
        {
            m_builder.end_array();
            m_state_machine.pop();
        }
        else if(!isws(c))
            throw std::runtime_error{"document failed: "s + c};
    }

    static bool isws(char c)
    {
        return (std::isblank(c) || std::iscntrl(c));
    }

    using state = std::function<void(decoder&,char)>;

    std::stack<state> m_state_machine = std::stack<state>{};

    Builder& m_builder;

    xson::string_type m_string = ""s;

    xson::integer_type m_integer = 0;

    xson::number_type m_number = 0;

    xson::integer_type m_place = 1;

}; // class decoder

} // namespace xson::json

#pragma once

#include <stack>
#include "gsl/not_null.hpp"
#include "xson/object.hpp"

namespace xson::json::_3 {

using namespace std::string_literals;

class observer
{
public:
    virtual void start_document() = 0;
    virtual void end_document() = 0;
    virtual void name(xson::string_type) = 0;
    virtual void index(std::size_t) = 0;
    virtual void value(xson::string_type) = 0;
    virtual void value(xson::boolean_type) = 0;
    virtual void value(xson::null_type) = 0;
    virtual void value(xson::integer_type) = 0;
    virtual void value(xson::number_type) = 0;
};

class builder : public observer
{
public:

    xson::object get()
    {
        return std::move(m_parent);
    }

private:

    void start_document()
    {
        if(m_stack.empty())
            m_stack.push(m_parent);
        else
            m_stack.push(m_stack.top().get()[m_current]);
    }

    void end_document()
    {
        m_stack.pop();
    }

    void name(xson::string_type str)
    {
        m_stack.top().get().type(xson::type::object);
        m_current = std::move(str);
    }

    void index(std::size_t i)
    {
        m_stack.top().get().type(xson::type::array);
        m_current = std::to_string(i);
    }

    void value(xson::string_type str)
    {
        m_stack.top().get()[m_current] = str;
    }

    void value(xson::boolean_type b)
    {
        m_stack.top().get()[m_current] = b;
    }

    void value(xson::null_type)
    {
        m_stack.top().get()[m_current] = nullptr;
    }

    void value(xson::integer_type i)
    {
        m_stack.top().get()[m_current] = i;
    }

    void value(xson::number_type d)
    {
        m_stack.top().get()[m_current] = d;
    }

    std::stack<std::reference_wrapper<xson::object>> m_stack = std::stack<std::reference_wrapper<xson::object>>{};

    xson::object m_parent = xson::object{};

    xson::string_type m_current = ""s;

}; // class builder

class parser
{
public:

    parser(gsl::not_null<observer*> o) : m_observer{o}
    {}

    void parse(std::istream& is)
    {
        m_state_machine.push(&parser::document);
        while(is)
        {
            m_state_machine.top()(*this,is.get());
            if(m_state_machine.empty()) break;
        }
        if(!m_state_machine.empty())
            throw std::runtime_error{"Invalid JSON object"s};
    }

    void parse(std::string_view sv)
    {
        m_state_machine.push(&parser::document);
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
        m_observer->value(Boolean);
        m_state_machine.pop();
        m_state_machine.top()(*this,c);
    }

    void null(char c)
    {
        m_observer->value(nullptr);
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
            m_observer->value(move(m_string));
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
            m_state_machine.pop();
            m_state_machine.push(&parser::fractions<Sign>);
        }
        else
        {
            m_observer->value(Sign * m_integer);
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
            m_observer->value(Sign * m_number);
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
            m_state_machine.push(&parser::string);
        }
        else if(isdigit(c))
        {
            m_state_machine.push(&parser::positive);
            m_state_machine.top()(*this,c);
        }
        else if(c == '-')
        {
            m_state_machine.push(&parser::negative);
        }
        else if(c == 't')
        {
            m_state_machine.push(&parser::constant<true>);
            m_state_machine.push(&parser::is<'e'>);
            m_state_machine.push(&parser::is<'u'>);
            m_state_machine.push(&parser::is<'r'>);
        }
        else if(c == 'f')
        {
            m_state_machine.push(&parser::constant<false>);
            m_state_machine.push(&parser::is<'e'>);
            m_state_machine.push(&parser::is<'s'>);
            m_state_machine.push(&parser::is<'l'>);
            m_state_machine.push(&parser::is<'a'>);
        }
        else if(c == 'n')
        {
            m_state_machine.push(&parser::null);
            m_state_machine.push(&parser::is<'l'>);
            m_state_machine.push(&parser::is<'l'>);
            m_state_machine.push(&parser::is<'u'>);
        }
        else if(c == '{' || c == '[')
        {
            m_state_machine.push(&parser::document);
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
            m_observer->name(m_string);
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
            m_state_machine.push(&parser::comma);
            m_state_machine.push(&parser::value);
            m_state_machine.push(&parser::colon);
            m_state_machine.push(&parser::name);
        }
        else if(!isws(c))
            throw std::runtime_error{"object failed: "s + c};
    }

    struct array
    {
        std::size_t m_index = 0;

        void operator()(parser& p, char c)
        {
            if(c == ']')
            {
                p.m_state_machine.pop();
                p.m_state_machine.top()(p,c);
            }
            else if (c != ',' && c != '}')
            {
                p.m_observer->index(m_index);
                ++m_index;
                p.m_state_machine.push(&parser::comma);
                p.m_state_machine.push(&parser::value);
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
            m_observer->start_document();
            m_state_machine.push(&parser::object);
        }
        else if(c == '[')
        {
            m_observer->start_document();
            m_state_machine.push(array{});
        }
        else if(c == '}' || c == ']')
        {
            m_observer->end_document();
            m_state_machine.pop();
        }
        else if(!isws(c))
            throw std::runtime_error{"document failed: "s + c};
    }

    static bool isws(char c)
    {
        return (std::isblank(c) || std::iscntrl(c));
    }

    using state = std::function<void(parser&,char)>;

    std::stack<state> m_state_machine = std::stack<state>{};

    gsl::not_null<observer*> m_observer;

    xson::string_type m_string = ""s;

    xson::integer_type m_integer = 0;

    xson::number_type m_number = 0;

    xson::integer_type m_place = 1;
}; // class parser

} // namespace xson::json::_2

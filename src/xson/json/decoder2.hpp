#pragma once

#include <stack>
#include "gsl/not_null.hpp"
#include "xson/object.hpp"

namespace xson::json::_2 {

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
    xson::object m_parent = xson::object{};

    std::stack<std::reference_wrapper<xson::object>> m_stack = std::stack<std::reference_wrapper<xson::object>>{};

    xson::string_type m_current = ""s;

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

public:

    xson::object get()
    {
        return std::move(m_parent);
    }

}; // class builder

class parser
{
    using state = std::function<void(char)>;

    std::stack<state> m_state_machine = std::stack<state>{};

    gsl::not_null<observer*> m_observer;

    struct inner
    {
    public:
        inner(gsl::not_null<parser*> p) : m_outer{p}
        {}
    protected:
        gsl::not_null<parser*> m_outer;
    };

    template <bool Boolean>
    struct constant : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            m_outer->m_observer->value(Boolean);
            m_outer->m_state_machine.pop();
            m_outer->m_state_machine.top()(c);
        }
    };

    struct null : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            m_outer->m_observer->value(nullptr);
            m_outer->m_state_machine.pop();
            m_outer->m_state_machine.top()(c);
        }
    };

    template <char Character>
    struct is : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            if(c == Character)
                m_outer->m_state_machine.pop();
            else
                throw std::runtime_error{"failed: "s + c + " is not " + Character};
        }
    };

    class string : public inner
    {
        xson::string_type m_value = ""s;
    public:
        using inner::inner;
        void operator()(char c)
        {
            if(c != '\"')
                m_value += c;
            else
            {
                m_outer->m_observer->value(move(m_value));
                m_outer->m_state_machine.pop();
            }
        }
    };

    template<int Sign>
    class fractions : public inner
    {
        xson::integer_type m_place = 1;
        xson::number_type m_value;
    public:
        fractions(parser* p, xson::integer_type initial) :
            inner{p},
            m_value{static_cast<xson::number_type>(initial)}
        {}
        void operator()(char c)
        {
            if(isdigit(c))
            {
                m_place *= 10;
                m_value += (static_cast<xson::number_type>(c-'0')/m_place);
            }
            else
            {
                m_outer->m_observer->value(Sign * m_value);
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.top()(c);
            }
        }
    };

    template<int Sign>
    class number : public inner
    {
        xson::integer_type m_value = 0;
    public:
        using inner::inner;
        void operator()(char c)
        {
            if(std::isdigit(c))
            {
                m_value *= 10;
                m_value += (c-'0');
            }
            else if(c == '.')
            {
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.push(fractions<Sign>{m_outer,m_value});
            }
            else
            {
                m_outer->m_observer->value(Sign * m_value);
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.top()(c);
            }
        }
    };

    using positive = number<1>;

    using negative = number<-1>;

    struct value : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            if(c == '\"')
            {
                m_outer->m_state_machine.push(string{m_outer});
            }
            else if(isdigit(c))
            {
                m_outer->m_state_machine.push(positive{m_outer});
                m_outer->m_state_machine.top()(c);
            }
            else if(c == '-')
            {
                m_outer->m_state_machine.push(negative{m_outer});
            }
            else if(c == 't')
            {
                m_outer->m_state_machine.push(constant<true>{m_outer});
                m_outer->m_state_machine.push(is<'e'>{m_outer});
                m_outer->m_state_machine.push(is<'u'>{m_outer});
                m_outer->m_state_machine.push(is<'r'>{m_outer});
            }
            else if(c == 'f')
            {
                m_outer->m_state_machine.push(constant<false>{m_outer});
                m_outer->m_state_machine.push(is<'e'>{m_outer});
                m_outer->m_state_machine.push(is<'s'>{m_outer});
                m_outer->m_state_machine.push(is<'l'>{m_outer});
                m_outer->m_state_machine.push(is<'a'>{m_outer});
            }
            else if(c == 'n')
            {
                m_outer->m_state_machine.push(null{m_outer});
                m_outer->m_state_machine.push(is<'l'>{m_outer});
                m_outer->m_state_machine.push(is<'l'>{m_outer});
                m_outer->m_state_machine.push(is<'u'>{m_outer});
            }
            else if(c == '{' || c == '[')
            {
                m_outer->m_state_machine.push(document{m_outer});
                m_outer->m_state_machine.top()(c);
            }
            else if (c == ',' || c == '}' || c == ']')
            {
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.top()(c);
            }
            else if(!isws(c))
                throw std::runtime_error{"value failed: "s + c};
        }
    };

    struct comma : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            if(c == ',')
            {
                m_outer->m_state_machine.pop();
            }
            else if(c == '}' || c == ']')
            {
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.top()(c);
            }
            else if(!isws(c))
                throw std::runtime_error{"comma failed: "s + c};
        }
    };

    struct colon : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            if(c == ':')
                m_outer->m_state_machine.pop();
            else if(!isws(c))
                throw std::runtime_error{"colon failed: "s + c};
        }
    };

    class name : public inner
    {
        xson::string_type value = ""s;
    public:
        using inner::inner;
        void operator()(char c)
        {
            if(c != '\"')
                value += c;
            else
            {
                m_outer->m_observer->name(move(value));
                m_outer->m_state_machine.pop();
            }
        }
    };

    struct object : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            if(c == '}')
            {
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.top()(c);
            }
            else if(c == '\"')
            {
                m_outer->m_state_machine.push(comma{m_outer});
                m_outer->m_state_machine.push(value{m_outer});
                m_outer->m_state_machine.push(colon{m_outer});
                m_outer->m_state_machine.push(name{m_outer});
            }
            else if(!isws(c))
                throw std::runtime_error{"object failed: "s + c};
        }
    };

    class array : public inner
    {
        std::size_t index = 0;
    public:
        using inner::inner;
        void operator()(char c)
        {
            if(c == ']')
            {
                m_outer->m_state_machine.pop();
                m_outer->m_state_machine.top()(c);
            }
            else if (c != ',' && c != '}')
            {
                m_outer->m_observer->index(index);
                ++index;
                m_outer->m_state_machine.push(comma{m_outer});
                m_outer->m_state_machine.push(value{m_outer});
                m_outer->m_state_machine.top()(c);
            }
            else if(!isws(c))
                throw std::runtime_error{"array failed: "s + c};
        }
    };

    struct document : public inner
    {
        using inner::inner;
        void operator()(char c)
        {
            if(c == '{')
            {
                m_outer->m_observer->start_document();
                m_outer->m_state_machine.push(object{m_outer});
            }
            else if(c == '[')
            {
                m_outer->m_observer->start_document();
                m_outer->m_state_machine.push(array{m_outer});
            }
            else if(c == '}' || c == ']')
            {
                m_outer->m_observer->end_document();
                m_outer->m_state_machine.pop();
            }
            else if(!isws(c))
                throw std::runtime_error{"document failed: "s + c};
        }
    };

    static bool isws(char c)
    {
        return (std::isblank(c) || std::iscntrl(c));
    }

public:

    parser(gsl::not_null<observer*> o) : m_observer{o}
    {}

    void parse(std::istream& is)
    {
        m_state_machine.push(document{this});
        auto c = char{};
        while(is && !m_state_machine.empty())
        {
            is >> std::noskipws >> c;
            m_state_machine.top()(c);
        }
        if(!m_state_machine.empty())
            throw std::runtime_error{"Invalid JSON object"s};
    }

}; // class parser

} // namespace xson::json::_2

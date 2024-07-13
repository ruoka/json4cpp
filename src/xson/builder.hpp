#pragma once

#include <exception>
#include <stack>
#include "xson/object.hpp"

namespace xson {

class builder
{
public:

    xson::object get()
    {
        return std::move(m_root);
    }

    void start_object()
    {
        TRACE('!');
        if(m_stack.empty())
        {
            m_root = object{object::map{}};
            m_stack.push(m_root);
        }
        else if(m_type == type::object)
        {
            m_stack.top().get().get<object::map>().emplace(m_current,object::map{});
            m_stack.push(m_stack.top().get().get<object::map>().at(m_current));
        }
        else // type::array
        {
            m_stack.top().get().get<object::array>().emplace_back(object::map{});
            m_stack.push(m_stack.top().get().get<object::array>().back());
        }
        m_type = type::object;
    }

    void end_object()
    {
        TRACE('!');
        m_stack.pop();
        if(not m_stack.empty())
        {
            if(m_stack.top().get().is_object())
                m_type = type::object;
            else if(m_stack.top().get().is_array())
                m_type = type::array;
            else
                std::terminate(); // error
        }
    }

    void start_array()
    {
        TRACE('!');
        if(m_stack.empty())
        {
            m_root = object{object::array{}};
            m_stack.push(m_root);
        }
        else if(m_type == type::object)
        {
            m_stack.top().get().get<object::map>().emplace(m_current,object::array{});
            m_stack.push(m_stack.top().get().get<object::map>().at(m_current));
        }
        else // type::array
        {
            m_stack.top().get().get<object::array>().emplace_back(object::array{});
            m_stack.push(m_stack.top().get().get<object::array>().back());
        }
        m_type = type::array;
    }

    void end_array()
    {
        TRACE('!');
        m_stack.pop();
        if(not m_stack.empty())
        {
            if(m_stack.top().get().is_object())
                m_type = type::object;
            else if(m_stack.top().get().is_array())
                m_type = type::array;
            else
                std::terminate(); // error
        }
    }

    void name(xson::string_type str)
    {
        TRACE(str);
        m_current = std::move(str);
    }

    template<Value T> requires (not Null<T>)
    void value(const T& val)
    {
        TRACE(s);
        if(m_type == type::object)
            m_stack.top().get().get<object::map>().emplace(m_current,val);
        else // type::array
            m_stack.top().get().get<object::array>().emplace_back(val);
    }

    template<Null T>
    void value(T)
    {
        TRACE(s);
        if(m_type == type::object)
            m_stack.top().get().get<object::map>().emplace(m_current,std::monostate{});
        else // type::array
            m_stack.top().get().get<object::array>().emplace_back(std::monostate{});
    }

private:

    using stack_type = std::stack<std::reference_wrapper<xson::object>>;

    enum class type {object,array};

    type m_type = type{};

    stack_type m_stack = stack_type{};

    xson::string_type m_current = ""s;

    xson::object m_root;

}; // class builder

} // namespace xson

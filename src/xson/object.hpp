#pragma once

#include <initializer_list>
#include <map>
#include "xson/type.hpp"
#include "xson/utility.hpp"

namespace xson {

class object
{
public:

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T>
    object(const std::enable_if_t<is_value<T>::value,std::string>& name, const T& value) : object()
    {
        object& ob = m_objects[name];
        ob.value(value);
    }

    object(const std::string& name, const object& ob) : object()
    {
        m_objects[name] = ob;
    }

    template <typename T, std::size_t N>
    object(const std::enable_if_t<std::is_same<T,object>::value,std::string>& name, const std::array<T,N>& array) : object()
    {
        object& parent = m_objects[name];
        parent.type(type::array);
        std::size_t idx{0};
        for(const auto& obj : array)
        {
            parent.m_objects[std::to_string(idx)] = obj;
            ++idx;
        }
    }

    template <typename T, typename A>
    object(const std::enable_if_t<std::is_same<T,object>::value,std::string>& name, const std::vector<T,A>& array) : object()
    {
        object& parent = m_objects[name];
        parent.type(type::array);
        std::size_t idx{0};
        for(const auto& obj : array)
        {
            parent.m_objects[std::to_string(idx)] = obj;
            ++idx;
        }
    }

    template <typename T>
    object(const std::enable_if_t<is_array<T>::value,std::string>& name, const T& array) : object()
    {
        object& parent = m_objects[name];
        parent.type(type::array);
        std::size_t idx{0};
        for(const auto& value : array)
        {
            object& child = parent.m_objects[std::to_string(idx)];
            child.value(value);
            ++idx;
        }
    }

    object(std::initializer_list<object> il) : object()
    {
        for(const auto& i : il)
            m_objects.insert(i.cbegin(), i.cend());
    }

    object(const object& ob) :
    m_value{ob.m_value}, m_type{ob.m_type}, m_objects{ob.m_objects}
    {}

    object(object&& ob) :
    m_value{std::move(ob.m_value)}, m_type{ob.m_type}, m_objects{std::move(ob.m_objects)}
    {}

    object& operator = (const object& ob)
    {
        m_value= ob.m_value; m_type = ob.m_type; m_objects = ob.m_objects;
        return *this;
    }

    object& operator = (object&& ob)
    {
        m_value = std::move(ob.m_value); m_type = ob.m_type; m_objects = std::move(ob.m_objects);
        return *this;
    }

    xson::type type() const
    {
        return m_type;
    }

    void type(xson::type t)
    {
        m_type = t;
    }

    const std::string& value() const
    {
        return m_value;
    }

    template <typename T>
    void value(const T& val)
    {
        m_type = xson::to_type(val);
        m_value = std::to_string(val);
    }

    object& operator [] (const std::string& name)
    {
        return m_objects[name];
    }

    object& operator [] (std::size_t idx)
    {
        return m_objects[std::to_string(idx)];
    }

    operator std::string () const
    {
        return m_value;
    }

    operator int () const
    {
        return std::stoi(m_value);
    }

    operator long long () const
    {
        return std::stol(m_value);;
    }

    operator double () const
    {
        return std::stod(m_value);;
    }

    operator bool () const
    {
        return std::stob(m_value);
    }

    operator std::chrono::system_clock::time_point () const
    {
        return std::chrono::system_clock::now(); // FIXME
    }

    bool empty () const
    {
        return m_objects.empty();
    }

    std::map<std::string,object>::const_iterator begin() const
    {
        return m_objects.cbegin();
    }

    std::map<std::string,object>::const_iterator end() const
    {
        return m_objects.cend();
    }

    std::map<std::string,object>::const_iterator cbegin() const
    {
        return m_objects.cbegin();
    }

    std::map<std::string,object>::const_iterator cend() const
    {
        return m_objects.cend();
    }

private:

    xson::type m_type;

    std::string m_value;

    std::map<std::string,object> m_objects;
};

} // namespace xson

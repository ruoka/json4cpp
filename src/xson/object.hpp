#pragma once

#include <initializer_list>
#include <map>
#include "std/utility.hpp"
#include "xson/type.hpp"

namespace xson {

using namespace std::string_literals;
using namespace std::chrono_literals;

class object
{
public:

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T>
    object(const std::enable_if_t<is_value<T>::value,std::string>& name, const T& value) : object()
    {
        m_objects[name].value(value);
    }

    template <typename T>
    object(const std::enable_if_t<is_value_array<T>::value,std::string>& name, const T& array) : object()
    {
        auto& parent = m_objects[name];
        auto idx = std::size_t{0};
        for(const auto& value : array)
        {
            parent.m_objects[std::to_string(idx)].value(value);
            ++idx;
        }
        parent.type(type::array);
    }

    object(const std::string& name, const object& obj) : object()
    {
        m_objects[name] = obj;
    }

    template <typename T>
    object(const std::enable_if_t<is_object_array<T>::value,std::string>& name, const T& array) : object()
    {
        auto& parent = m_objects[name];
        auto idx = std::size_t{0};
        for(const auto& ob : array)
        {
            parent.m_objects[std::to_string(idx)] = ob;
            ++idx;
        }
        parent.type(type::array);
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
    const T& value(const T& value)
    {
        m_type = xson::to_type(value);
        m_value = std::to_string(value);
        return value;
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
        return std::stotp(m_value);
    }

    object& operator + (const object& obj)
    {
        m_objects.insert(obj.cbegin(), obj.cend());
        return *this;
    }

    bool empty () const
    {
        return m_objects.empty();
    }

    bool has(const std::string& name) const
    {
        return m_objects.count(name) > 0;
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

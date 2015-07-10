#pragma once

#include <initializer_list>
#include <string>
#include <chrono>
#include <array>
#include <vector>
#include <map>
#include "std/utility.hpp"

namespace xson {

namespace json {

    class decoder;

    class encoder;
}

template <typename T> struct is_value : std::false_type {};

template <> struct is_value<int> : std::true_type {};

template <> struct is_value<long> : std::true_type {};

template <> struct is_value<double> : std::true_type {};

template <> struct is_value<bool> : std::true_type {};

template <> struct is_value<std::string> : std::true_type {};

template <> struct is_value<std::chrono::system_clock::time_point> : std::true_type {};

template <> struct is_value<std::nullptr_t> : std::true_type {};

template <typename T> using is_array = std::is_array<T>;

class object
{
public:

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T>
    object(const std::enable_if_t<is_value<T>::value,std::string>& name, const T& value) : object()
    {
        object& obj = m_objects[name];
        obj.m_type = to_type(value);
        obj.m_value = to_value(value);
    }

    object(const std::string& name, const object& obj) : object()
    {
        m_objects[name] = obj;
    }

    template <typename T>
    object(const std::enable_if_t<is_array<T>::value,std::string>& name, const T& array) : object()
    {
        object& parent = m_objects[name];
        parent.m_type = type::array;
        std::size_t idx{0};
        for(const auto& i : array)
        {
            object& child = parent.m_objects[to_name(idx++)];
            child.m_type = to_type(i);
            child.m_value = to_value(i);
        }
    }

    object(std::initializer_list<object> il) : object()
    {
        for(const auto& i : il)
            m_objects.insert(i.m_objects.cbegin(), i.m_objects.cend());
    }

    object(const object& obj) :
    m_value{obj.m_value}, m_type{obj.m_type}, m_objects{obj.m_objects}
    {}

    object(object&& obj) :
    m_value{std::move(obj.m_value)}, m_type{obj.m_type}, m_objects{std::move(obj.m_objects)}
    {}

    object& operator = (const object& obj)
    {
        m_value= obj.m_value; m_type = obj.m_type; m_objects = obj.m_objects;
        return *this;
    }

    object& operator = (object&& obj)
    {
        m_value = std::move(obj.m_value); m_type = obj.m_type; m_objects = std::move(obj.m_objects);
        return *this;
    }

    object& operator [] (const std::string& name)
    {
        return m_objects[name];
    }

    object& operator [] (std::size_t idx)
    {
        return m_objects[to_name(idx)];
    }

    operator std::string () const
    {
        return m_value;
    }

    operator int () const
    {
        return std::stoi(m_value);
    }

    operator long () const
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

    bool empty () const
    {
        return m_objects.empty();
    }

    std::map<std::string,object>::iterator begin()
    {
        return m_objects.begin();
    }

    std::map<std::string,object>::iterator end()
    {
        return m_objects.end();
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

    enum class type
    {
        object,
        array,
        string,
        number,
        boolean,
        null
    };

    template <typename T>
    static std::string to_name(const T& idx)
    {
        return std::to_string(idx);
    }

    template <typename T>
    static type to_type(const T&);

    template <typename T>
    static std::string to_value(const T& val)
    {
        return std::to_string(val);
    }

    type m_type;

    std::string m_value;

    std::map<std::string,object> m_objects;

    friend class xson::json::decoder;

    friend class xson::json::encoder;
};

template <> inline object::type object::to_type(const std::string&)
{
    return object::type::string;
}

template <> inline object::type object::to_type(const int&)
{
    return object::type::number;
}

template <> inline object::type object::to_type(const long&)
{
    return object::type::number;
}

template <> inline object::type object::to_type(const double&)
{
    return object::type::number;
}

template <> inline object::type object::to_type(const bool&)
{
    return object::type::boolean;
}

template <> inline object::type object::to_type(const std::chrono::system_clock::time_point&)
{
    return object::type::string;
}

template <> inline object::type object::to_type(const std::nullptr_t&)
{
    return object::type::null;
}

} // namespace xson

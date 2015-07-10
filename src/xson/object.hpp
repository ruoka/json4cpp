#pragma once

#include <initializer_list>
#include <string>
#include <chrono>
#include <array>
#include <vector>
#include <map>
#include "xson/utility.hpp"

namespace xson {

namespace json {

    class decoder;

    class encoder;
}

namespace bson {

    class decoder;

    class encoder;
}

template <typename T> struct is_value : std::false_type {};

template <> struct is_value<std::int32_t> : std::true_type {};

template <> struct is_value<std::int64_t> : std::true_type {};

template <> struct is_value<double> : std::true_type {};

template <> struct is_value<bool> : std::true_type {};

template <> struct is_value<std::string> : std::true_type {};

template <> struct is_value<std::chrono::system_clock::time_point> : std::true_type {};

template <> struct is_value<std::nullptr_t> : std::true_type {};

template <typename T> using is_array = std::is_array<T>;

class object
{
public:

    enum class type : std::int32_t
    {
        // json
        number                = '\x01',
        string                = '\x02',
        object                = '\x03',
        array                 = '\x04',
        boolean               = '\x08',
        null                  = '\x0A',

        // + bson
        binary                = '\x05',
        undefined             = '\x06', // Deprecated
        objectid              = '\x07',
        date                  = '\x09',
        regular_expression    = '\x0B',
        db_pointer            = '\x0C', // Deprecated
        javascript            = '\x0D',
        deprecated            = '\x0E',
        javascript_with_scope = '\x0F',
        int32                 = '\x10',
        timestamp             = '\x11',
        int64                 = '\x12',
        min_key               = '\xFF',
        max_key               = '\x7F'
    };

    friend std::ostream& operator << (std::ostream& os, type t)
    {
        os << static_cast<int>(t);
        return os;
    }

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T>
    object(const std::enable_if_t<is_value<T>::value,std::string>& name, const T& value) : object()
    {
        object& ob = m_objects[name];
        ob.m_type = to_type(value);
        ob.m_value = to_value(value);
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

    object::type get_type() const
    {
        return m_type;
    }

    const std::string& value() const
    {
        return m_value;
    }

    template <typename T>
    void value(const T& val)
    {
        m_value = to_value(val);
        m_type = to_type(val);
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

    template <typename T>
    static std::string to_name(const T& idx)
    {
        return std::to_string(idx);
    }

    template <typename T>
    static constexpr type to_type(const T&);

    template <typename T>
    static std::string to_value(const T& val)
    {
        return std::to_string(val);
    }

    type m_type;

    std::string m_value;

    std::map<std::string,object> m_objects;

//    friend class json::decoder;

//    friend class json::encoder;

//    friend class bson::decoder;

//    friend class bson::encoder;
};

template <> inline void object::value(const object::type& type)
{
    m_type = type;
}

template <> inline object::type object::to_type(const double&)
{
    return object::type::number;
}

template <> inline object::type object::to_type(const std::string&)
{
    return object::type::string;
}

template <> inline object::type object::to_type(const bool&)
{
    return object::type::boolean;
}

template <> inline object::type object::to_type(const std::nullptr_t&)
{
    return object::type::null;
}

template <> inline object::type object::to_type(const std::chrono::system_clock::time_point&)
{
    return object::type::date;
}

template <> inline object::type object::to_type(const int&)
{
    return object::type::int32;
}

template <> inline object::type object::to_type(const long long&)
{
    return object::type::int64;
}

} // namespace xson

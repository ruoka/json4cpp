#pragma once

#include <initializer_list>
#include <map>
#include <cmath>
#include "std/variant.hpp"
#include "std/extension.hpp"
#include "xson/type.hpp"

namespace xson {

using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace std::experimental;

using  value = variant<std::double_t,   // \x01
                       std::string_t,   // \x02
                       std::bool_t,     // \x08
                       std::datetime_t, // \x09
                       std::nullptr_t,  // \x0A
                       std::int32_t,    // \x10
                       std::int64_t     // \x12
                       >;

inline auto to_string(const value& val)
{
    if(holds_alternative<std::string_t>(val))
        return get<std::string_t>(val);
    if(holds_alternative<std::double_t>(val))
        return std::to_string(get<std::double_t>(val));
    if(holds_alternative<std::bool_t>(val))
        return std::to_string(get<std::bool_t>(val));
    if(holds_alternative<std::datetime_t>(val))
        return std::to_string(get<std::datetime_t>(val));
    if(holds_alternative<std::nullptr_t>(val))
        return std::to_string(get<std::nullptr_t>(val));
    if(holds_alternative<std::int32_t>(val))
        return std::to_string(get<std::int32_t>(val));
    if(holds_alternative<std::int64_t>(val))
        return std::to_string(get<std::int64_t>(val));
    throw std::logic_error{"This type is not supported"};
}

struct less
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        const auto has_only_digits = lhs.find_first_not_of("0123456789") == std::string::npos &&
                                     rhs.find_first_not_of("0123456789") == std::string::npos;
        if(has_only_digits)
        {
            if(lhs.size() == rhs.size())
                return lhs < rhs;
            else
                return lhs.size() < rhs.size();
        }
        return lhs < rhs;
    }
};

class object
{
public:

    using const_iterator = std::map<std::string_t,object>::const_iterator;

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T,
              typename = std::enable_if_t<!is_object_v<T>      &&
                                          !is_value_array_v<T> &&
                                          !is_object_array_v<T>>>
    object(const std::string_t& name, const T& val) :
    object{}
    {
        static_assert(is_value_v<T>, "This type is not supported");
        m_objects[name].value(val);
    }

    template <typename T>
    object(const std::enable_if_t<is_value_array_v<T>,std::string_t>& name, const T& array) :
    object{}
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

    template <typename T>
    object(const std::enable_if_t<is_value_v<T>,std::string_t>& name, std::initializer_list<T> array) :
    object{name, std::vector<T>{array}}
    {}

    object(const std::string& name, const object& obj) :
    object{}
    {
        m_objects[name] = obj;
    }

    template <typename T>
    object(const std::enable_if_t<is_object_array_v<T>,std::string_t>& name, const T& array) :
    object{}
    {
        auto& parent = m_objects[name];
        auto idx = std::size_t{0};
        for(const auto& obj : array)
        {
            parent.m_objects[std::to_string(idx)] = obj;
            ++idx;
        }
        parent.type(type::array);
    }

    object(std::initializer_list<object> il) :
    object{}
    {
        for(const auto& i : il)
            m_objects.insert(i.cbegin(), i.cend());
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

    template <typename T,
              typename = std::enable_if_t<!is_object_v<T>      &&
                                          !is_value_array_v<T> &&
                                          !is_object_array_v<T>>>
    object& operator = (const T& val)
    {
        static_assert(is_value_v<T>, "This type is not supported");
        value(val);
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

    const value& value() const
    {
        return m_value;
    }

    template <typename T>
    void value(const T& val)
    {
        static_assert(is_value_v<T>, "This type is not supported");
        m_type = xson::to_type(val);
        m_value = val;
    }

    object& operator [] (const std::string_t& name)
    {
        return m_objects[name];
    }

    const object& operator [] (const std::string_t& name) const
    {
        if(!m_objects.count(name))
            throw std::out_of_range("object has no field with name "s + name);
        return m_objects.find(name)->second;
    }

    object& operator [] (std::size_t idx)
    {
        if(m_objects.empty())
            m_type = type::array;
        return m_objects[std::to_string(idx)];
    }

    const object& operator [] (std::size_t idx) const
    {
        const auto name = std::to_string(idx);
        if(!m_objects.count(name))
            throw std::out_of_range("array has no index with value "s + name);
        return m_objects.find(name)->second;
    }

    operator const xson::value& () const
    {
        return m_value;
    }

    operator std::double_t () const
    {
        return get<double>(m_value);
    }

    operator const std::string_t& () const
    {
        return get<std::string_t>(m_value);
    }

    operator std::bool_t () const
    {
        return get<std::bool_t>(m_value);
    }

    operator std::datetime_t () const
    {
        return get<std::datetime_t>(m_value);
    }

    operator std::nullptr_t () const
    {
        return get<std::nullptr_t>(m_value);
    }

    operator std::int32_t () const
    {
        return get<std::int32_t>(m_value);
    }

    operator std::int64_t () const
    {
        return get<std::int64_t >(m_value);
    }

    operator std::vector<std::string> () const
    {
        if(m_type != type::array)
            throw std::logic_error("object type has to be array");
        auto values = std::vector<std::string>{};
        for(const auto& obj : m_objects)
            values.push_back(obj.second); // FIXME We assume string values hare
        return values;
    }

    object& operator + (const object& obj)
    {
        if(m_type == type::object)
            m_objects.insert(obj.cbegin(), obj.cend());
        else if(m_type == type::array)
            m_objects[std::to_string(m_objects.size())] = obj;
        return *this;
    }

    object& operator + (object&& obj)
    {
        if(m_type == type::object)
            m_objects.insert(obj.cbegin(), obj.cend());
        else if(m_type == type::array)
            m_objects[std::to_string(m_objects.size())] = obj;
        return *this;
    }

    object& operator += (const object& obj)
    {
        *this = *this + obj;
        return *this;
    }

    object& operator += (object&& obj)
    {
        *this = *this + std::move(obj);
        return *this;
    }

    bool has_value () const
    {
        return m_value.index() != variant_npos;
    }

    bool empty () const
    {
        return m_objects.empty();
    }

    bool has(const std::string& name) const
    {
        return m_objects.count(name) > 0;
    }

    bool match(const object& subset) const
    {
        if(subset.empty() && !subset.has_value())
            return true;
        if(subset.type() == type::object || subset.type() == type::array)
        {
            auto lf = m_objects.cbegin();
            auto ll = m_objects.cend();
            auto rf = subset.m_objects.cbegin();
            auto rl = subset.m_objects.cend();
            while(lf != ll && rf != rl)
            {
                if(lf->first < rf->first)
                    ++lf;
                else if(lf->first > rf->first)
                    return false;
                else if(lf->second.match(rf->second)) {
                    ++lf;
                    ++rf;
                }
                else
                    return false;
            }
            return rf == rl;
        }
        else if(m_type == type::int64 && subset.m_type == type::int32)
            return get<std::int64_t>(m_value) == get<std::int32_t>(subset.m_value);
        else
            return m_value == subset.m_value;
    }

    std::size_t size() const
    {
        return m_objects.size();
    }

    const_iterator begin() const
    {
        return m_objects.cbegin();
    }

    const_iterator end() const
    {
        return m_objects.cend();
    }

    const_iterator cbegin() const
    {
        return m_objects.cbegin();
    }

    const_iterator cend() const
    {
        return m_objects.cend();
    }

private:

    xson::type m_type;

    xson::value m_value;

    std::map<std::string,object,less> m_objects;
};

} // namespace xson

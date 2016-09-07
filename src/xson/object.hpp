#pragma once

#include <initializer_list>
#include <map>
#include "std/extension.hpp"
#include "xson/type.hpp"

namespace xson {

using namespace std::string_literals;
using namespace std::chrono_literals;

class object
{
public:

    using const_iterator = std::map<std::string,object>::const_iterator;

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T,
              typename = std::enable_if_t<!is_object<T>::value       &&
                                          !is_value_array<T>::value  &&
                                          !is_object_array<T>::value >>
    object(const std::string& name, const T& value) :
    object{}
    {
        static_assert(is_value<T>::value, "Invalid type!");
        m_objects[name].value(value);
    }

    template <typename T>
    object(const std::enable_if_t<is_value_array<T>::value,std::string>& name, const T& array) :
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
    object(const std::enable_if_t<is_value<T>::value,std::string>& name, std::initializer_list<T> array) :
    object{name, std::vector<T>{array}}
    {}

    object(const std::string& name, const object& obj) :
    object{}
    {
        m_objects[name] = obj;
    }

    template <typename T>
    object(const std::enable_if_t<is_object_array<T>::value,std::string>& name, const T& array) :
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
              typename = std::enable_if_t<!is_object<T>::value       &&
                                          !is_value_array<T>::value  &&
                                          !is_object_array<T>::value >>
    object& operator = (const T& val)
    {
        static_assert(is_value<T>::value, "Invalid type!");
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

    const std::string& value() const
    {
        return m_value;
    }

    template <typename T>
    const T& value(const T& value)
    {
        static_assert(is_value<T>::value, "Invalid type!");
        m_type = xson::to_type(value);
        m_value = std::to_string(value);
        return value;
    }

    object& operator [] (const std::string& name)
    {
        return m_objects[name];
    }

    const object& operator [] (const std::string& name) const
    {
        if(!m_objects.count(name))
            throw std::out_of_range("object has no element with name "s + name);
        return m_objects.find(name)->second;
    }

    object& operator [] (std::size_t idx)
    {
        return m_objects[std::to_string(idx)];
    }

    const object& operator [] (std::size_t idx) const
    {
        const auto name = std::to_string(idx);
        if(!m_objects.count(name))
            throw std::out_of_range("array has no index "s + name);
        return m_objects.find(name)->second;
    }

    operator std::string () const
    {
        return m_value;
    }

    operator int () const
    {
        if(m_type != type::int32 && m_type != type::number)
            throw std::logic_error("object type has to be int32");
        return std::stoi(m_value);
    }

    operator long long () const
    {
        if(m_type != type::int32 && m_type != type::int64 && m_type != type::number)
            throw std::logic_error("object type has to be int64");
        return std::stol(m_value);;
    }

    operator double () const
    {
        if(m_type != type::number && m_type != type::int32 && m_type != type::int64)
            throw std::logic_error("object type has to be double");
        return std::stod(m_value);;
    }

    operator bool () const
    {
        if(m_type != type::boolean)
            throw std::logic_error("object type has to be an boolean");
        return std::stob(m_value);
    }

    operator std::chrono::system_clock::time_point () const
    {
        if(m_type != type::date)
            throw std::logic_error("object type has to be date");
        return std::stotp(m_value);
    }

    operator std::vector<std::string> () const
    {
        if(m_type != type::array)
            throw std::logic_error("object type has to be array");
        auto values = std::vector<std::string>{};
        for(const auto& obj : m_objects)
            values.push_back(obj.second);
        return values;
    }

    object& operator + (object&& obj)
    {
        if(m_type == type::object)
            m_objects.insert(obj.cbegin(), obj.cend());
        else if(m_type == type::array)
            m_objects[std::to_string(m_objects.size())] = obj;
        return *this;
    }

    object& operator += (object&& obj)
    {
        *this = *this + std::move(obj);
        return *this;
    }

    bool has_value () const
    {
        return !m_value.empty();
    }

    bool empty () const
    {
        return m_value.empty() && m_objects.empty();
    }

    bool has(const std::string& name) const
    {
        return m_objects.count(name) > 0;
    }

    bool match(const object& subset) const
    {
        if(subset.empty())
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
        else
            return value() == subset.value();
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

    std::string m_value;

    std::map<std::string,object> m_objects;
};

} // namespace xson

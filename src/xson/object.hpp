#pragma once

#include <iostream>
#include <initializer_list>
#include <functional>
#include <map>
#include "std/variant.hpp"
#include "std/extension.hpp"
#include "xson/type.hpp"

namespace xson {

using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace std::experimental;

using  value = variant<null_type,    // \x0A
                       number_type,  // \x01
                       string_type,  // \x02
                       boolean_type, // \x08
                       date_type,    // \x09
                       int32_type,   // \x10
                       int64_type    // \x12
                       >;

inline auto to_string(const value& val)
{
    if(holds_alternative<string_type>(val))
        return get<string_type>(val);
    if(holds_alternative<number_type>(val))
        return std::to_string(get<number_type>(val));
    if(holds_alternative<boolean_type>(val))
        return std::to_string(get<boolean_type>(val));
    if(holds_alternative<date_type>(val))
        return std::to_string(get<date_type>(val));
    if(holds_alternative<null_type>(val))
        return std::to_string(get<null_type>(val));
    if(holds_alternative<int32_type>(val))
        return std::to_string(get<int32_type>(val));
    if(holds_alternative<int64_type>(val))
        return std::to_string(get<int64_type>(val));
    throw std::logic_error{"This type is not supported"};
}

struct less
{
    bool operator()(const string_type& lhs, const string_type& rhs) const
    {
        if(ext::numeric(lhs) && ext::numeric(rhs))
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

    using const_iterator = std::map<string_type,object>::const_iterator;

    object() : m_type{type::object}, m_value{}, m_objects{}
    {}

    template <typename T,
             std::enable_if_t<!is_object_v<T>      &&
                              !is_value_array_v<T> &&
                              !is_object_array_v<T>,bool> = true>
    object(const string_type& name, const T& val) :
    object{}
    {
        static_assert(is_value_v<T>, "This type is not supported");
        m_objects[name].value(val);
    }

    template <typename T,
              std::enable_if_t<is_value_array_v<T>,bool> = true>
    object(const string_type& name, const T& array) :
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

    template <typename T,
              std::enable_if_t<is_value_v<T>,bool> = true>
    object(const string_type& name, std::initializer_list<T> vil) :
    object{name, std::vector<T>{vil}}
    {}

    object(const std::string& name, const object& obj) :
    object{}
    {
        m_objects[name] = obj;
    }

    template <typename T,
              std::enable_if_t<is_object_array_v<T>,bool> = true>
    object(const string_type& name, const T& array) :
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

    object(std::initializer_list<object> oil) :
    object{}
    {
        for(const auto& o : oil)
            m_objects.insert(o.cbegin(), o.cend());
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

    const xson::value& value() const
    {
        return m_value;
    }

    template <typename T>
    void value(const T& val)
    {
        static_assert(is_value_v<T>, "This type is not supported");
        m_type = to_type(val);
        m_value = val;
    }

    object& operator [] (const string_type& name)
    {
        return m_objects[name];
    }

    const object& operator [] (const string_type& name) const
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

    operator number_type () const
    {
        return get<double>(m_value);
    }

    operator const string_type& () const
    {
        return get<string_type>(m_value);
    }

    operator boolean_type () const
    {
        return get<boolean_type>(m_value);
    }

    operator date_type () const
    {
        return get<date_type>(m_value);
    }

    operator null_type () const
    {
        return get<null_type>(m_value);
    }

    operator int32_type () const
    {
        return get<int32_type>(m_value);
    }

    operator int64_type () const
    {
        return get<int64_type >(m_value);
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

    bool has_value() const
    {
        return m_value.index();
    }

    bool has_objects() const
    {
        return !m_objects.empty();
    }

    bool empty() const
    {
        return !(has_value() || has_objects());
    }

    bool has(const std::string& name) const
    {
        return m_objects.count(name) > 0;
    }

    bool match(const object& subset) const
    {
        if(subset.empty())
            return true;

        if(has_value() && subset.has_objects())
        {
            auto test = std::all_of(subset.cbegin(), subset.cend(),
                [&](const auto& node)
                {
                    if(operators.count(node.first))
                        return operators.at(node.first)(value(), node.second.value());
                    else
                        return node.first[0] == '$';
                });

            if(subset.has("$in"s))
                test = test && std::any_of(subset["$in"s].cbegin(), subset["$in"s].cend(),
                    [&](const auto& node)
                    {
                        return value() == node.second.value();
                    });

            return test;
        }

        if(has_objects() && subset.has_objects())
        {
            auto lf = cbegin(), rf = subset.cbegin();
            const auto ll = cend(), rl = subset.cend();

            while(rf != rl && rf->first[0] == '$')
                ++rf;

            while(lf != ll && rf != rl)
            {
                if(lf->first < rf->first)
                    ++lf;
                else if(lf->first > rf->first)
                    return false;
                else if(lf->second.match(rf->second))
                {
                    ++lf;
                    ++rf;
                }
                else
                    return false;
            }
            return rf == rl;
        }

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

    using operator_type = std::function<bool(const xson::value&,const xson::value&)>;

    const std::map<string_type,operator_type> operators = std::map<string_type,operator_type>{
        { "$eq"s,  std::equal_to<xson::value>{}      },
        { "$ne"s,  std::not_equal_to<xson::value>{}  },
        { "$lt"s,  std::less<xson::value>{}          },
        { "$lte"s, std::less_equal<xson::value>{}    },
        { "$gt"s,  std::greater<xson::value>{}       },
        { "$gte"s, std::greater_equal<xson::value>{} }
    };

    xson::type m_type;

    xson::value m_value;

    std::map<string_type,object_type,less> m_objects;
};

} // namespace xson

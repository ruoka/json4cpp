#pragma once
#include <iostream>
#include <initializer_list>
#include <functional>
#include <map>
#include <variant>
//#include <ranges>
#include "xson/type.hpp"
#include "xson/trace.hpp"
#include "std/extension.hpp"

namespace xson {

using namespace std::string_literals;
using namespace std::chrono_literals;
using monostate = std::monostate;

class object
{
public:

    using map = std::map<std::string,object>;
    using array = std::vector<object>;
    using value = std::variant<monostate,   // \x0A
                              number_type,  // \x01
                              string_type,  // \x02
                              date_type,    // \x09
                              integer_type, // \x12
                              boolean_type  // \x08
                              >;
    using data = std::variant<map,array,value>;

    object() : m_data{}
    {
        TRACE('!');
    }

    object(const data& d) : m_data{d}
    {
        TRACE('!');
    }

    template <typename T> requires (std::is_constructible_v<value,T>  && !std::is_null_pointer_v<T>)
    object(const string_type& name, const T& v) :
    object{}
    {
        TRACE('!');
        auto o = object{};
        o.m_data = value{v};
        m_data = map{{name,o}};
    }

    template <typename T> requires std::is_null_pointer_v<T>
    object(const string_type& name, const T&) :
    object{}
    {
        TRACE('!');
        auto o = object{};
        o.m_data = value{};
        m_data = map{{name,o}};
    }

    template <typename T> requires xson::is_value_array_v<T>
    object(const string_type& name, const T& values) :
    object{}
    {
        TRACE('!');
        m_data = map{};
        auto arr = array{};
        for(const auto& v : values)
            arr.emplace_back(v);
        std::get<map>(m_data).emplace(name,arr);
    }

    template <typename T> requires std::is_constructible_v<value,T>
    object(const string_type& name, std::initializer_list<T> values)
    {
        TRACE('!');
        m_data = map{};
        auto arr = array{};
        for(auto& v : values)
            arr.emplace_back(v);
        std::get<map>(m_data).emplace(name,arr);
    }

    object(const std::string& name, const object& o) :
    object{}
    {
        TRACE('!');
        m_data = map{};
        std::get<map>(m_data).emplace(name,o);
    }

    object(std::initializer_list<object> objects)
    {
        TRACE('!');
        m_data = map{};
        for(auto& o : objects)
            std::get<map>(m_data).insert(std::get<map>(o.m_data).cbegin(),std::get<map>(o.m_data).end());
    }

    template <typename T> requires xson::is_object_array_v<T>
    object(const string_type& name, const T& objects) :
    object{}
    {
        TRACE('!');
        m_data = map{};
        auto arr = array{};
        for(const auto& o : objects)
            arr.emplace_back(o);
        std::get<map>(m_data).emplace(name,arr);
    }

    object(const object& obj) :
    m_data{obj.m_data}
    {
        TRACE('!');
    }

    object(object&& obj) :
    m_data{std::move(obj.m_data)}
    {
        TRACE('!');
    }

    object& operator = (const object& o)
    {
        m_data = o.m_data;
        return *this;
    }

    object& operator = (object&& obj)
    {
        m_data = std::move(obj.m_data);
        return *this;
    }

    object& operator = (const data& d)
    {
        m_data = d;
        return *this;
    }

    template <typename T> requires std::is_null_pointer_v<T>
    object& operator = (const T&)
    {
        m_data = monostate{};
        return *this;
    }

    template <typename T> requires (std::is_constructible_v<value,T> && !std::is_null_pointer_v<T>)
    object& operator = (const T& val)
    {
        m_data = val;
        return *this;
    }

    xson::type type() const
    {
        if(holds_alternative<map>(m_data))
            return xson::type::object;

        if(holds_alternative<array>(m_data))
            return xson::type::array;

        const auto& data = std::get<value>(m_data);

        if(holds_alternative<number_type>(data))
            return xson::type::number;

        if(holds_alternative<string_type>(data))
            return xson::type::string;

        if(holds_alternative<boolean_type>(data))
            return xson::type::boolean;

        if(holds_alternative<date_type>(data))
            return xson::type::date;

        if(holds_alternative<integer_type>(data))
            return xson::type::integer;

        // if(holds_alternative<monostate>(data))
        return xson::type::null;
    }

    template<typename T>
        requires std::is_same_v<T,value>
    const auto& get() const
    {
        return std::get<value>(m_data);
    }

    template<typename T>
        requires std::is_same_v<T,value>
    auto& get()
    {
        return std::get<value>(m_data);
    }

    template<typename T>
        requires std::is_same_v<T,map>
    const auto& get() const
    {
        return std::get<map>(m_data);
    }

    template<typename T>
        requires std::is_same_v<T,map>
    auto& get()
    {
        return std::get<map>(m_data);
    }

    template<typename T>
        requires std::is_same_v<T,array>
    const auto& get() const
    {
        return std::get<array>(m_data);
    }

    template<typename T>
        requires std::is_same_v<T,array>
    auto& get()
    {
        return std::get<array>(m_data);
    }

    object& operator [] (const string_type& name)
    {
        return std::get<map>(m_data)[name];
    }

    const object& operator [] (const string_type& name) const
    {
        if(not std::get<map>(m_data).count(name))
            throw std::out_of_range("object has no field with name "s + name);
        return std::get<map>(m_data).find(name)->second;
    }

    object& operator [] (std::size_t idx)
    {
        return std::get<array>(m_data)[idx];
    }

    const object& operator [] (std::size_t idx) const
    {
        if(idx > std::get<array>(m_data).size())
            throw std::out_of_range("array has no index with value "s + std::to_string(idx));
        return std::get<array>(m_data)[idx];
    }

    operator const object::value& () const
    {
        return std::get<value>(m_data);
    }

    operator number_type () const
    {
        return std::get<number_type>(std::get<value>(m_data));
    }

    operator const string_type& () const
    {
        return std::get<string_type>(std::get<value>(m_data));
    }

    operator boolean_type () const
    {
        return std::get<boolean_type>(std::get<value>(m_data));
    }

    operator date_type () const
    {
        return std::get<date_type>(std::get<value>(m_data));
    }

    operator null_type () const
    {
        return nullptr;
    }

    operator int32_type () const
    {
        return std::get<integer_type >(std::get<value>(m_data));
    }

    operator integer_type () const
    {
        return std::get<integer_type >(std::get<value>(m_data));
    }

    object& operator + (const object& obj)
    {
        if(holds_alternative<map>(m_data))
            std::get<map>(m_data).insert(std::get<map>(obj.m_data).cbegin(), std::get<map>(obj.m_data).cend());
        else if(holds_alternative<array>(m_data))
            std::get<array>(m_data).push_back(obj);
        return *this;
    }

    object& operator + (object&& obj)
    {
        if(holds_alternative<map>(m_data))
            std::get<map>(m_data).insert(std::get<map>(obj.m_data).cbegin(), std::get<map>(obj.m_data).cend());
        else if(holds_alternative<array>(m_data))
            std::get<array>(m_data).push_back(obj);
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
        return holds_alternative<value>(m_data);
    }

    bool has_objects() const
    {
        return holds_alternative<map>(m_data);
    }

    bool is_array() const
    {
        return holds_alternative<array>(m_data);
    }

    bool empty() const
    {
        if(holds_alternative<map>(m_data))
            return std::get<map>(m_data).empty();
        if(holds_alternative<array>(m_data))
            return std::get<array>(m_data).empty();
        else
            return false;
    }

    bool has(const std::string& name) const
    {
        if(holds_alternative<map>(m_data))
            return std::get<map>(m_data).contains(name);
        else
            return false;
    }


    std::size_t size() const
    {
        if(holds_alternative<map>(m_data))
            return std::get<map>(m_data).size();
        else if(holds_alternative<array>(m_data))
            return std::get<array>(m_data).size();
        else
            return 0u;
    }

    auto count() const
    {
        return size();
    }

    bool match([[maybe_unused]] const object& subset) const
    {
        // if(subset.empty())
        //     return true;
        //
        // if(has_value() && subset.has_objects())
        // {
        //     auto test = std::all_of(subset.cbegin(), subset.cend(),
        //         [&](const auto& node)
        //         {
        //             if(operators.count(node.first))
        //                 return operators.at(node.first)(value(), node.second.value());
        //             else
        //                 return node.first[0] == '$';
        //         });
        //
        //     if(subset.has("$in"s))
        //         test = test && std::any_of(subset["$in"s].cbegin(), subset["$in"s].cend(),
        //             [&](const auto& node)
        //             {
        //                 return value() == node.second.value();
        //             });
        //
        //     return test;
        // }
        //
        // if(has_objects() && subset.has_objects())
        // {
        //     auto lf = cbegin(), rf = subset.cbegin();
        //     const auto ll = cend(), rl = subset.cend();
        //
        //     while(rf != rl && rf->first[0] == '$')
        //         ++rf;
        //
        //     while(lf != ll && rf != rl)
        //     {
        //         if(lf->first < rf->first)
        //             ++lf;
        //         else if(lf->first > rf->first)
        //             return false;
        //         else if(lf->second.match(rf->second))
        //         {
        //             ++lf;
        //             ++rf;
        //         }
        //         else
        //             return false;
        //     }
        //     return rf == rl;
        // }
        //
        // return m_value == subset.m_value;

        return true; // FIXME
    }

private:

    using operator_type = std::function<bool(const object::value&,const object::value&)>;

    const std::map<string_type,operator_type> operators = std::map<string_type,operator_type>{
        { "$eq"s,  std::equal_to<object::value>{}      },
        { "$ne"s,  std::not_equal_to<object::value>{}  },
        { "$lt"s,  std::less<object::value>{}          },
        { "$lte"s, std::less_equal<object::value>{}    },
        { "$gt"s,  std::greater<object::value>{}       },
        { "$gte"s, std::greater_equal<object::value>{} }
    };

    data m_data;
};

using std::to_string;
using ext::to_string;

inline std::string to_string(const object::value& val)
{
    return std::visit([](const auto& arg){return xson::to_string(arg);}, val);
}

} // namespace xson

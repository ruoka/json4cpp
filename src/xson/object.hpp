#pragma once
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <functional>
#include <map>
#include <variant>
#include <charconv>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <ranges>
#include "xson/concepts.hpp"
#include "xson/trace.hpp"

namespace xson {

using namespace std::string_literals;
using namespace std::chrono_literals;
using monostate = std::monostate;

class object
{
public:

    using map = std::map<std::string,object>;
    using array = std::vector<object>;
    using value = std::variant<monostate,     // \x0A
                              number_type,    // \x01
                              string_type,    // \x02
                              timestamp_type, // \x09
                              integer_type,   // \x12
                              boolean_type    // \x08
                              >;
    using data = std::variant<map,array,value>;

    object() : m_data{}
    {
        TRACE('!');
    }

    explicit object(const data& d) : m_data{d}
    {
        TRACE('!');
    }

    template <typename T> requires (Value<T> and not Null<T>)
    object(const string_type& name, const T& v) :
    object{}
    {
        TRACE('!');
        static_assert(Value<T>);
        auto o = object{};
        o.m_data = value{v};
        m_data = map{{name,o}};
    }

    template <Null T>
    object(const string_type& name, [[maybe_unused]] const T& v) :
    object{}
    {
        TRACE('!');
        auto o = object{};
        o.m_data = value{};
        m_data = map{{name,o}};
    }

    template <Value T>
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
        TRACE("! xxxxxxx");
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

    template <typename T> requires (not Value<T> and not Object<T> and std::ranges::forward_range<T>)
    object(const string_type& name, T&& data) :
    object{}
    {
        TRACE('!');
        m_data = map{};
        auto arr = array{};
        for(const auto& d : data)
            arr.emplace_back(d);
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
        TRACE('!');
        m_data = o.m_data;
        return *this;
    }

    object& operator = (object&& obj)
    {
        TRACE('!');
        m_data = std::move(obj.m_data);
        return *this;
    }

    object& operator = (const array& arr)
    {
        TRACE('!');
        m_data = arr;
        return *this;
    }

    template <Value T> requires (not Null<T>)
    object& operator = (const T& val)
    {
        TRACE('!');
        m_data = val;
        return *this;
    }

    template <Null T>
    object& operator = (const T&)
    {
        TRACE('!');
        m_data = monostate{};
        return *this;
    }

    friend bool operator == (const object& lhs, const object& rhs)
    {
        return lhs.m_data == rhs.m_data;
    }

    constexpr auto is_object() const
    {
        return holds_alternative<map>(m_data);
    }

    constexpr auto is_array() const
    {
        return holds_alternative<array>(m_data);
    }

    constexpr auto is_number() const
    {
        if(holds_alternative<value>(m_data))
            return holds_alternative<number_type>(std::get<value>(m_data));
        return false;
    }

    constexpr auto is_string() const
    {
        if(holds_alternative<value>(m_data))
            return holds_alternative<string_type>(std::get<value>(m_data));
        return false;
    }

    constexpr auto is_boolean() const
    {
        if(holds_alternative<value>(m_data))
            return holds_alternative<boolean_type>(std::get<value>(m_data));
        return false;
    }

    constexpr auto is_timestamp() const
    {
        if(holds_alternative<value>(m_data))
            return holds_alternative<timestamp_type>(std::get<value>(m_data));
        return false;
    }

    constexpr auto is_integer() const
    {
        if(holds_alternative<value>(m_data))
            return holds_alternative<integer_type>(std::get<value>(m_data));
        return false;
    }

    constexpr auto is_null() const
    {
        if(holds_alternative<value>(m_data))
            return holds_alternative<std::monostate>(std::get<value>(m_data));
        return false;
    }

    template<typename T> requires std::same_as<T,value>
    const auto& get() const
    {
        return std::get<value>(m_data);
    }

    template<typename T> requires std::same_as<T,value>
    auto& get()
    {
        return std::get<value>(m_data);
    }

    template<typename T> requires std::same_as<T,map>
    const auto& get() const
    {
        return std::get<map>(m_data);
    }

    template<typename T> requires std::same_as<T,map>
    auto& get()
    {
        return std::get<map>(m_data);
    }

    template<typename T> requires std::same_as<T,array>
    const auto& get() const
    {
        return std::get<array>(m_data);
    }

    template<typename T> requires std::same_as<T,array>
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
        if(std::get<map>(m_data).contains(name))
            return std::get<map>(m_data).find(name)->second;
        throw std::out_of_range("object has no field with name "s + name);
    }

    object& operator [] (std::size_t idx)
    {
        return std::get<array>(m_data)[idx];
    }

    const object& operator [] (std::size_t idx) const
    {
        if(idx < std::get<array>(m_data).size())
            return std::get<array>(m_data)[idx];
        throw std::out_of_range("array has no index with value "s + std::to_string(idx));
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

    operator timestamp_type () const
    {
        return std::get<timestamp_type>(std::get<value>(m_data));
    }

//    operator null_type () const
//    {
//        return nullptr;
//    }

    operator integer_type () const
    {
        return std::get<integer_type >(std::get<value>(m_data));
    }

    operator int32_type () const
    {
        return std::get<integer_type>(std::get<value>(m_data));
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
            std::get<map>(m_data).merge(std::get<map>(obj.m_data));
        else if(holds_alternative<array>(m_data))
            std::get<array>(m_data).push_back(std::move(obj));
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

    bool match(const object& subset) const
    {
        if(subset.empty())
            return true;

        if(is_array() and subset.is_array())
        {
            if(get<array>().size() != subset.get<array>().size())
                return false;

            for(auto i = 0ll; const auto &o : subset.get<array>())
                if(get<array>()[i++].get<value>() != o.get<value>()) return false;

            return true;
        }

        if(has_value() and subset.has_objects())
        {
            auto test = std::ranges::all_of(subset.get<map>(),
                [&](const auto& node)
                {
                    if(operators.count(node.first))
                        return operators.at(node.first)(get<value>(), node.second.template get<value>());
                    else
                        return node.first[0] == '$';
                });

            if(subset.has("$in"s))
                test = test && std::any_of(subset["$in"s].get<map>().cbegin(), subset["$in"s].get<map>().cend(),
                    [&](const auto& node)
                    {
                        return get<value>() == node.second.template get<value>();
                    });

            return test;
        }

        if(has_objects() and subset.has_objects())
        {
            auto lf = get<map>().cbegin(), rf = subset.get<map>().cbegin();
            const auto ll = get<map>().cend(), rl = subset.get<map>().cend();

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

        return get<value>() == subset.get<value>();
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

template<typename T>
auto to_iso8601(const std::chrono::time_point<T>& current_time) noexcept
{
    const auto midnight = std::chrono::floor<std::chrono::days>(current_time);
    const auto date = std::chrono::year_month_day{midnight};
    const auto time = std::chrono::hh_mm_ss{current_time - midnight};
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time.subseconds());
    // YYYY-MM-DDThh:mm:ss.fffZ
    auto os = std::ostringstream{};
    os << std::setw(4) << std::setfill('0') << (int)date.year()        << '-'
       << std::setw(2) << std::setfill('0') << (unsigned)date.month()  << '-'
       << std::setw(2) << std::setfill('0') << (unsigned)date.day()    << 'T'
       << std::setw(2) << std::setfill('0') << time.hours().count()    << ':'
       << std::setw(2) << std::setfill('0') << time.minutes().count()  << ':'
       << std::setw(2) << std::setfill('0') << time.seconds().count()  << '.'
       << std::setw(3) << std::setfill('0') << milliseconds.count()    << 'Z';
    return os.str();
}

inline auto to_time_point(const std::string_view sv)
{
    // YYYY-MM-DDTHH:MM:SS.sssZ
    assert(sv.size() > 9);
    auto YYYY = 0; auto MM = 0u, DD = 0u, hh = 0u, mm = 0u, ss = 0u, fff = 0u;
    auto res = std::from_chars_result{sv.data(),std::errc()};
    res = std::from_chars(res.ptr,res.ptr+4,YYYY);
    res = std::from_chars(++res.ptr,res.ptr+2,MM);
    res = std::from_chars(++res.ptr,res.ptr+2,DD);
    if(sv.length() == 24)
    {
        res = std::from_chars(++res.ptr,res.ptr+2,hh);
        res = std::from_chars(++res.ptr,res.ptr+2,mm);
        res = std::from_chars(++res.ptr,res.ptr+2,ss);
        res = std::from_chars(++res.ptr,res.ptr+3,fff);
    }
    using namespace std::chrono;
    return sys_days{year{YYYY}/month{MM}/day{DD}} + hours{hh} + minutes{mm} + seconds{ss} + milliseconds{fff};
}

using std::to_string;

inline const std::string to_string(std::monostate) noexcept
{
    return "null";
}

template<typename T>
std::string to_string(const std::chrono::time_point<T>& point) noexcept
{
    return to_iso8601(point);
}

inline const std::string to_string(bool b) noexcept
{
    return b ? "true" : "false";
}

constexpr const std::string& to_string(const std::string& str) noexcept
{
    return str;
}

inline std::string to_string(const object::value& val)
{
    return std::visit([](const auto& arg){return xson::to_string(arg);}, val);
}

using array = xson::object::array;

} // namespace xson

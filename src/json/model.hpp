#pragma once

#include <initializer_list>
#include <sstream>

namespace json
{

struct element
{
    template <typename V>
    element(const std::string& key, const V& value)
    {
        serial << '\"' << key << '\"' << ":";
        if (std::is_same<V,std::string>::value) serial << '\"';
        serial << std::boolalpha << value;
        if (std::is_same<V,std::string>::value) serial << '\"';
    }
    friend std::ostream& operator << (std::ostream& os, const element& e)
    {
        return os << e.serial.str();
    }
private:
    std::ostringstream serial;
};

struct array
{
    array()
    {
        serial << "[]";
    }
    template <typename V>
    array(std::initializer_list<V> il)
    {
        serial << '[';
        for(auto it = il.begin(); it != il.end(); ++it)
        {
            if (it != il.begin()) serial << ',';
            serial << *it;
        }
        serial << ']';
    }
    friend std::ostream& operator << (std::ostream& os, const array& a)
    {
        return os << a.serial.str();
    }
private:
    std::ostringstream serial;
};

struct document
{
    document()
    {
        serial << "{}";
    }
    template <typename V>
    document(const std::string& key, const V& value)
    {
        serial << '{' << element{key, value} << '}';
    }
    document(std::initializer_list<element> il)
    {
        serial << '{';
        for(auto it = il.begin(); it != il.end(); ++it)
        {
            if (it != il.begin()) serial << ',';
            serial << *it;
        }
        serial << '}';
    }
    friend std::ostream& operator << (std::ostream& os, const document& d)
    {
        return os << d.serial.str();
    }
private:
    std::ostringstream serial;
};

} // namespace json

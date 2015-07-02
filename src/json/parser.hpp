#pragma once

#include <map>
#include <string>
#include <iosfwd>

namespace json
{

class object
{
public:

    object() :
    value{}, value_type{type::object}, objects{}
    {}

    object(std::istream& is) : object()
    {
        is >> std::skipws;
        parse_document(is,*this);
    }

    object(object&& c) :
    value{std::move(c.value)}, value_type{c.value_type}, objects{std::move(c.objects)}
    {}

    object& operator [] (const std::string& name)
    {
        return objects[name];
    }

    object& operator [] (std::size_t idx)
    {
        return objects[std::to_string(idx)];
    }

    operator std::string () const
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    operator int () const
    {
        int i;
        std::stringstream ss;
        ss << value;
        ss >> i;
        return i;
    }

    operator long () const
    {
        long l;
        std::stringstream ss;
        ss << value;
        ss >> l;
        return l;
    }

    operator double () const
    {
        double d;
        std::stringstream ss;
        ss << value;
        ss >> d;
        return d;
    }

    operator bool () const
    {
        bool b;
        std::stringstream ss;
        ss << value;
        ss >> std::boolalpha >> b;
        return b;
    }

    bool empty () const
    {
        return objects.empty();
    }

    friend std::ostream& operator << (std::ostream&, const object&);

private:

    void parse_string(std::istream& is, object& result);

    void parse_number(std::istream& is, object& result);

    void parse_array(std::istream& is, object& result);

    void parse_document(std::istream& is, object& result);

    enum class type
    {
        string,
        number,
        object,
        array,
        boolean,
        null
    };

    std::string value;

    type value_type;

    std::map<std::string,object> objects;

    object(const object&) = delete;
    object& operator = (const object&) = delete;
    object& operator = (object&&) = delete;
};

void object::parse_string(std::istream& is, object& result)
{
    char next;
    std::string value;
    is >> next; // "
    getline(is, value, '\"'); // value"
    result.value = value;
    result.value_type = type::string;
}

void object::parse_number(std::istream& is, object& result)
{
    char next;
    std::string value;
    is >> next;
    while (next != ',' && next != '}' && next != ']')
    {
        value += next;
        is >> next;
    }
    is.putback(next); // , }, or ] do not belong to the number
    result.value = value;
    result.value_type = type::number;
}

void object::parse_array(std::istream& is, object& result)
{
    std::size_t idx{0};
    char next;
    is >> next; // [

    while(next != ']' && is)
    {
        std::string name{std::to_string(idx++)};

        is >> std::ws;
        next = is.peek();

        if (next == '{')
        {
            parse_document(is, result.objects[name]);
            is >> next; // , or ]
        }
        else if (next == '[')
        {
            parse_array(is, result.objects[name]);
            is >> next; // , or ]
        }
        else if (next == '\"')
        {
            parse_string(is, result.objects[name]);
            is >> next; // , or ]
        }
        else
        {
            parse_number(is, result.objects[name]);
            is >> next; // , }, or ]
        }
    }
    result.value_type = type::array;
}

void object::parse_document(std::istream& is, object& result)
{
    char next;
    is >> next; // {

    while(next != '}' && is)
    {
        std::string name, value;
        is >> std::ws >> next; // "
        getline(is, name, '\"'); // name"
        is >> next; // :

        is >> std::ws;
        next = is.peek();

        if(next == '{')
        {
            parse_document(is, result.objects[name]);
            is >> next; // , or }
        }
        else if(next == '[')
        {
            parse_array(is, result.objects[name]);
            is >> next; // , or }
        }
        else if(next == '\"')
        {
            parse_string(is, result.objects[name]);
            is >> next; // , or ]
        }
        else
        {
            parse_number(is, result.objects[name]);
            is >> next; // , }, or ]
        }
    }
    result.value_type = type::object;
}

object parse(std::istream& is)
{
    return object{is};
}

std::ostream& operator << (std::ostream& os, const object& c)
{
    if(!c.objects.empty() && c.value_type == object::type::object)
    {
        os << '{';
        for(auto it = c.objects.cbegin(); it != c.objects.cend(); ++it)
        {
            if (it != c.objects.cbegin()) os << ',';
            os << '\"' << it->first << '\"' << ':' << it->second;
        }
        os << '}';
    }
    else if(!c.objects.empty() && c.value_type == object::type::array)
    {
        os << '[';
        for(auto it = c.objects.cbegin(); it != c.objects.cend(); ++it)
        {
            if (it != c.objects.cbegin()) os << ',';
            os << it->second;
        }
        os << ']';
     }
    else if(!c.value.empty() && c.value_type == object::type::string)
        os << '\"' << c.value << '\"';
    else if(!c.value.empty())
        os << c.value;
    else if (c.value_type == object::type::object || c.value_type == object::type::array)
        os << "{}";
    else
        os << "null";

    return os;
}

} // namespace json

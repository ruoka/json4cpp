#pragma once

#include <map>
#include <string>
#include <iosfwd>
#include "bson/type.hpp"
#include "std/trace.hpp"
#include "std/utility.hpp"

namespace bson
{

struct object
{
    object() :
    value{}, value_type{0x03}, objects{}
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
        return std::stoi(value);
    }

    operator long () const
    {
        return std::stol(value);;
    }

    operator double () const
    {
        return std::stod(value);;
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

    template <typename T>
    friend int32_type parse_value(std::istream& is, object& result);

    friend int32_type parse_document(std::istream& is, object& result);

    std::string value;

    int32_type value_type;

    std::map<std::string,object> objects;

    object(const object&) = delete;
    object& operator = (const object&) = delete;
    object& operator = (object&&) = delete;
};

template <typename T>
int32_type parse_value(std::istream& is, object& result)
{
    T val;
    is.read(reinterpret_cast<char*>(&val), sizeof(val));
    result.value = std::to_string(val);
    result.value_type = bson::type(val);

    TRACE("   val: "  << result.value << "; size: " << sizeof(val));

    return is.gcount();
}

template<>
int32_type parse_value<std::chrono::system_clock::time_point>(std::istream& is, object& result)
{
    int64_type val1;
    is.read(reinterpret_cast<char*>(&val1), sizeof(val1));
    auto val2 = std::chrono::system_clock::from_time_t(val1);
    result.value = std::to_string(val2);
    result.value_type = bson::type(val2);

    TRACE("   val: "  << result.value << "; size: " << sizeof(val1));

    return is.gcount();
}

template<>
int32_type parse_value<std::nullptr_t>(std::istream& is, object& result)
{
    std::nullptr_t val;
    result.value = std::to_string(val);
    result.value_type = bson::type(val);

    TRACE("   val: "  << result.value << "; size: " << 0);

    return 0;
}

template<>
int32_type parse_value<std::string>(std::istream& is, object& result)
{
    int32_type bytes;
    is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));

    std::string val;
//    std::getline(is, val, '\x00');
    while(--bytes)
        val += is.get();

    is.ignore(1);
    bytes -= is.gcount();

    result.value = val;
    result.value_type = bson::type(val);

    TRACE("   val: "  << result.value << "; size: " << val.size());

    return sizeof(bytes) + val.size() + 1;
}

int32_type parse_document(std::istream& is, object& result)
{
    int32_type bytes;
    is.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    const auto length = sizeof(bytes) + bytes;

    TRACE("length: " << length);
    TRACE("bytes: "  << bytes);

    while(bytes > 1)
    {
        int32_type type;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        bytes -= is.gcount();

        std::string name;
        std::getline(is, name, '\x00');
        bytes -= name.size();
        --bytes;

        TRACE("key: " << name);

        switch(type)
        {
        case 0x01:
            bytes -= parse_value<double>(is, result.objects[name]);
            break;
        case 0x02:
            bytes -= parse_value<std::string>(is, result.objects[name]);
            break;
        case 0x03:
        case 0x04:
            bytes -= parse_document(is, result.objects[name]);
            result.objects[name].value_type = type;
            break;
        case 0x08:
            bytes -= parse_value<bool>(is, result.objects[name]);
            break;
        case 0x09:
            bytes -= parse_value<std::chrono::system_clock::time_point>(is, result.objects[name]);
            break;
        case 0x0A:
            bytes -= parse_value<std::nullptr_t>(is, result.objects[name]);
            break;
        case 0x10:
            bytes -= parse_value<std::int32_t>(is, result.objects[name]);
            break;
        case 0x12:
            bytes -= parse_value<std::int64_t>(is, result.objects[name]);
            break;
        default:
            assert(false && "This type is not supported yet");
            break;
        }

        TRACE("bytes: " << bytes);
    }

    is.ignore(1);
    bytes -= is.gcount();

    TRACE("bytes: " << bytes);

    return length;
}

object parse(std::istream& is)
{
    return object{is};
}

std::ostream& operator << (std::ostream& os, const object& c)
{
    if(!c.objects.empty() && c.value_type == 0x03)
    {
        os << '{';
        for(auto it = c.objects.cbegin(); it != c.objects.cend(); ++it)
        {
            if (it != c.objects.cbegin()) os << ',';
            os << '\"' << it->first << '\"' << ':' << std::boolalpha << it->second;
        }
        os << '}';
    }
    else if(!c.objects.empty() && c.value_type == 0x04)
    {
        os << '[';
        for(auto it = c.objects.cbegin(); it != c.objects.cend(); ++it)
        {
            if (it != c.objects.cbegin()) os << ',';
            os << std::boolalpha << it->second;
        }
        os << ']';
     }
    else if(!c.value.empty() && c.value_type == 0x02)
        os << '\"' << c.value << '\"';
    else if(!c.value.empty())
        os << std::boolalpha << c.value;
    else if (c.value_type == 0x03 || c.value_type == 0x04)
        os << "{}";
    else
        os << "null";

    return os;
}

} // namespace bson

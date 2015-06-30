#pragma once

#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include "bson/decoder.hpp"

namespace bson
{
    struct objectid
    {
        byte_type epoch[4];
        byte_type machine[3];
        byte_type process[2];
        byte_type counter[3];
    };

    struct binary
    {
        int32_type size;
        int32_type subtype;
        byte_type* bytes;
    };

    struct element : public decoder
    {
        template <typename V>
        element(const std::string& name, const V& value)
        {
            decode(type(value));
            decode(name);
            decode(value);
        }
    };

    struct array : public decoder
    {
        array()
        {
            decode(int32_type{0}); // bytes
        }
        template <typename V>
        array(std::initializer_list<V> il) : array()
        {
            for(auto& i : il)
                decode(i);
        }
    };

    struct document : public decoder
    {
        document()
        {
            decode(int32_type{0}); // bytes
        }
        template <typename V>
        document(const std::string& key, const V& value)
        {
            decode(element{key, value});
        }
        document(std::initializer_list<element> il) : document()
        {
            for(auto& i : il)
                decode(i);
        }
        friend std::ostream& operator << (std::ostream& os, const document&)
        {
            return os;
        }
    };

} // namespace bson

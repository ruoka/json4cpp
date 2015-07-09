#pragma once

#include <initializer_list>
#include <string>
#include <iosfwd>
#include "bson/decoder.hpp"

namespace bson
{
/*
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

    struct timestamp
    {
        byte_type increment[4];
        byte_type timestamp[4];
    };
*/
    struct element : public decoder
    {
        template <typename V>
        element(const std::string& name, const V& value)
        {
            decode(type(value));
            decode(name,true);
            decode(value);
        }

        template <typename V>
        element(int32_type idx, const V& value) : element(std::to_string(idx), value)
        {}
    };

    struct array : public decoder
    {
        array()
        {
            decode(int32_type{0}); // 0 bytes
        }

        template <typename V>
        array(std::initializer_list<V> values)
        {
            int32_type idx{0};
            for(auto& value : values)
                decode(element{idx++,value});
        }
    };

    struct document : public decoder
    {
        document()
        {
            decode(int32_type{0}); // 0 bytes
        }

        template <typename V>
        document(const std::string& name, const V& value)
        {
            decode(element{name, value});
        }

        document(std::initializer_list<element> elements)
        {
            for(auto& element : elements)
                decode(element);
        }

        friend std::ostream& operator << (std::ostream& os, const document& doc);
    };

} // namespace bson

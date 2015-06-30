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
        element(const cstring_type& name, const V& value)
        {
            decode(type(value));
            decode(name);
            decode(value);
        }

        template <typename V>
        element(int32_type idx, const V& value)
        {
            decode(idx); // FIXME?
            decode(value);
        }
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
        document(const std::string& key, const V& value)
        {
            decode(element{key, value});
        }

        document(std::initializer_list<element> elements)
        {
            for(auto& element : elements)
                decode(element);
        }

        friend std::ostream& operator << (std::ostream& os, const document& doc)
        {
            decoder dec;
            dec.decode(doc);
            os.write(dec.cbegin(), dec.size());
            return os;
        }
    };

} // namespace bson

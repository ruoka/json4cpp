#pragma once

namespace xson::fson {

enum class type : char8_t 
{
// json types

    object    = '\x01',
    array     = '\x02',
    string    = '\x03',
    number    = '\x04',
    boolean   = '\x05',
    null      = '\x06',

// additional types

    integer   = '\x11',
    timestamp = '\x12',

// control types

    name      = '\x1A',
    end       = '\x1B'
};

inline auto& operator << (std::ostream& os, type t)
{
    os << static_cast<int>(t);
    return os;
}

} // namespace xson:fson

#include <iostream>
#include "bson/decoder.hpp"
#include "bson/model.hpp"
#include "xson/trace.hpp"

namespace bson
{

decoder::decoder() : buffer{}
{}

void decoder:: decode(byte_type b)
{
    TRACE("(byte)");
    put(b);
}

void decoder:: decode(int32_type i)
{
    TRACE("(int32)");
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
}

void decoder::decode(int64_type i)
{
    TRACE("(int64)");
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
    put((i >> 32) & 0xFF);
    put((i >> 40) & 0xFF);
    put((i >> 48) & 0xFF);
    put((i >> 56) & 0xFF);
}

void decoder::decode(double_type d)
{
    TRACE("(double)");
    union{
        double_type d64;
        int64_type i64;
    } d2i;
    d2i.d64 = d;
    decode(d2i.i64);
}

void decoder::decode(boolean_type b)
{
    TRACE("(bool)");
    if(b) put('\x01');
    else  put('\x00');
}

void decoder::decode(date_type d)
{
    using namespace std::chrono;
    TRACE("(date)");
    const int64_type i = duration_cast<milliseconds>(d.time_since_epoch()).count();
    decode(i);
}

void decoder::decode(null_type b)
{
    TRACE("(null)");
}

void decoder::decode(const string_type& str, bool csting)
{
    TRACE("(string)    " << boolalpha << csting);
    if(!csting) decode(static_cast<int32_type>(str.size()+1)); // bytes
    for(byte_type b : str)            // data
        put(b);
    put('\x00');                      // 0
}

void decoder::decode(const element& e)
{
    TRACE("(element)");
    put(e.cbegin(), e.cend());
}

void decoder::decode(const array& a)
{
    TRACE("(array)");
    decoder l;
    l.decode(a.size()+1);
    put(l.cbegin(), l.cend()); // bytes
    put(a.cbegin(), a.cend()); // data
    put('\x00');               // 0
}

void decoder::decode(const document& d)
{
    TRACE("(document)");
    decoder l;
    l.decode(d.size()+1);
    put(l.cbegin(), l.cend()); // bytes
    put(d.cbegin(), d.cend()); // data
    put('\x00');               // 0
}

void decoder::put(char b)
{
    buffer.emplace_back(b);
    TRACE("(byte)     " << setw(5) << buffer.size() << setw(5) << b << " " << setw(5) << hex << uppercase << (int)b << dec);
}

void decoder::put(const_iterator begin, const_iterator end)
{
    buffer.insert(buffer.cend(), begin, end);
    using namespace std;
    TRACE("(begin,end)" << setw(5) << buffer.size());
}

std::ostream& operator << (std::ostream& os, const document& doc)
{
    decoder dec;
    dec.decode(doc);
    os.write(dec.data(), dec.size());
    return os;
}

} // namespace bson

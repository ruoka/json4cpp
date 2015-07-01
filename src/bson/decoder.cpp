#include <iostream>
#include <iomanip>
#include "bson/decoder.hpp"
#include "bson/model.hpp"

namespace bson
{

decoder::decoder() : buffer{}
{}

void decoder:: decode(int32_type i)
{
    std::clog << __func__ << "(int32)" << std::endl;
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
}

void decoder::decode(int64_type i)
{
    std::clog << __func__ << "(int64)" << std::endl;
    put(i & 0xFF);
    put((i >>  8) & 0xFF);
    put((i >> 16) & 0xFF);
    put((i >> 24) & 0xFF);
    put((i >> 32) & 0xFF);
    put((i >> 40) & 0xFF);
    put((i >> 48) & 0xFF);
    put((i >> 56) & 0xFF);
}

void decoder::decode(boolean_type b)
{
    std::clog << __func__ << "(bool)" << std::endl;
    if(b) put('\x01');
    else  put('\x00');
}

void decoder::decode(double_type d)
{
    std::clog << __func__ << "(double)" << std::endl;
    union{
        double_type d64;
        int64_type i64;
    } d2i;
    d2i.d64 = d;
    decode(d2i.i64);
}

void decoder::decode(const cstring_type& str)
{
    std::clog << __func__ << "(cstring)" << std::endl;
    for(byte_type b : str)
        put(b);
    put('\x00');
}

void decoder::decode(const element& e)
{
    std::clog << __func__ << "(element)" << std::endl;
    put(e.cbegin(), e.cend());
}

void decoder::decode(const array& a)
{
    std::clog << __func__ << "(array)" << std::endl;
    decoder l;
    l.decode(a.size());
    put(l.cbegin(), l.cend());
    put(a.cbegin(), a.cend());
    put('\x00');
}

void decoder::decode(const document& d)
{
    std::clog << __func__ << "(document)" << std::endl;
    decoder l;
    l.decode(d.size());
    put(l.cbegin(), l.cend());
    put(d.cbegin(), d.cend());
    put('\x00');
}

void decoder::put(char b)
{
    buffer.emplace_back(b);
    using namespace std;
    clog << buffer.size() << " " << __func__ << "(byte)" << " " << setw(5) << b << " " << setw(5) << hex << uppercase << (int)b << dec << endl;
}

void decoder::put(const_iterator begin, const_iterator end)
{
    buffer.insert(buffer.cend(), begin, end);
    using namespace std;
    clog << buffer.size() << " " << __func__ << "(begin,end)" << endl;
}

} // namespace bson

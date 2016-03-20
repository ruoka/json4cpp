#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace std
{

namespace chrono
{

using days = duration<int, ratio_multiply<hours::period, ratio<24>>::type>;

using years = duration<int, ratio_multiply<days::period, ratio<365>>::type>;

using months = duration<int, ratio_divide<years::period, ratio<12>>::type>;

inline constexpr tuple<years,months,days> split(const days& ds) noexcept
{
    static_assert(std::numeric_limits<unsigned>::digits >= 18,
             "This algorithm has not been ported to a 16 bit unsigned integer");
    static_assert(std::numeric_limits<int>::digits >= 20,
             "This algorithm has not been ported to a 16 bit signed integer");
    auto const z = ds.count() + 719468;
    auto const era = (z >= 0 ? z : z - 146096) / 146097;
    auto const doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
    auto const yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
    auto const y = static_cast<int>(yoe) + era * 400;
    auto const doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
    auto const mp = (5*doy + 2)/153;                                   // [0, 11]
    auto const d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
    auto const m = mp + (mp < 10 ? 3 : -9u);                           // [1, 12]
    return make_tuple(years{y + (m <= 2)}, months{m}, days{d});
}

} // namespace chrono

inline std::string to_string(chrono::system_clock::time_point tp)
{
    using namespace chrono;

    auto dd = duration_cast<days>(tp.time_since_epoch());
    years YY;
    months MM;
    days DD;
    tie(YY,MM,DD) = split(dd);
    tp -= dd;
    auto hh = duration_cast<hours>(tp.time_since_epoch());
    tp -= hh;
    auto mm = duration_cast<minutes>(tp.time_since_epoch());
    tp -= mm;
    auto ss = duration_cast<seconds>(tp.time_since_epoch());
    tp -= ss;
    auto ff = duration_cast<milliseconds>(tp.time_since_epoch());

    std::ostringstream os;
    os << YY.count()
       << '-' << std::setw(2) << std::setfill('0')
       << MM.count()
       << '-' << std::setw(2) << std::setfill('0')
       << DD.count()
       << 'T'
       << hh.count()
       << ':' << std::setw(2) << std::setfill('0')
       << mm.count()
       << ':' << std::setw(2) << std::setfill('0')
       << ss.count()
       << '.' << std::setw(3) << std::setfill('0')
       << ff.count()
       << 'Z';

    return os.str();
}

inline std::string to_string(bool b)
{
    std::stringstream ss;
    ss << std::boolalpha << b;
    return ss.str();
}

inline std::string to_string(std::nullptr_t)
{
    return "null";
}

inline const std::string& to_string(const string& str)
{
    return str;
}

inline bool stob(const string& str)
{
    bool b;
    stringstream ss;
    ss << str;
    ss >> boolalpha >> b;
    if(!ss) throw std::invalid_argument{"No conversion could be performed for '"s + str + "'"s};
    return b;
}

template <typename T, int N> struct is_array<std::array<T,N>> : std::true_type {};

template <typename T> struct is_array<std::vector<T>> : std::true_type {};

} // namespace std

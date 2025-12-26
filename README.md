# json4cpp
JSON and FSON Library for C++23

A modern C++23 module-based library for JSON and FSON (Fast JSON) serialization. Built with C++ modules for fast compilation and clean interfaces.

## Requirements

- C++23 compiler with modules support (**Clang 20+ required**; build scripts default to `clang++-20`)
- LLVM 20+ with libc++ `std.cppm` module support (for the `std` module)

## Building

The project uses the C++ Builder (CB) build system. From the project root:

```bash
tools/CB.sh <path-to-std.cppm> your-program.c++
```

## Quick start

### Create + stringify

```cpp
import std;
import xson;

using namespace std::string_literals;
using namespace xson;

auto document = object{
    { "Name"s, "Papa"s },
    { "Age"s,  40      },
    { "LuckyNumbers"s, {2, 22, 2112} },
    { "Lucky"s, false }
};

std::clog << json::stringify(document) << std::endl;
```

### Parse from a stream (`json::parse` or `operator>>`)

```cpp
import std;
import xson;

using namespace std::string_literals;
using namespace xson;
using xson::json::operator <<;
using xson::json::operator >>;

auto ss = std::stringstream{R"(
    {
        "_id" : 2,
        "Name" : "Ruoka",
        "Embedded" : {
            "_id" : 5,
            "Name" : "Tuma"
        },
        "Lucky Numbers" : [
            2,
            22,
            2112
        ]
    }
)"};

std::clog << ss.str() << "\n\n";

// Option A: explicit parse
auto result = json::parse(ss);

// Option B: stream operator (equivalent to json::parse(ss))
// auto result = xson::object{};
// ss >> result;

std::clog << std::setw(2) << result << "\n\n";

std::clog << "_id            = " << result["_id"s]               << "\n"
     << "Name           = " << result["Name"s]              << "\n"
          << "Embedded.Name  = " << result["Embedded"s]["Name"s] << "\n"
     << "Lucky Number 1 = " << result["Lucky Numbers"s][0]  << "\n"
     << "Lucky Number 2 = " << result["Lucky Numbers"s][1]  << "\n"
     << "Lucky Number 3 = " << result["Lucky Numbers"s][2]  << "\n\n";

xson::integer_type id = result["_id"s];
xson::string_type name = result["Name"s];
xson::integer_type number = result["Lucky Numbers"s][1];
```

### Standalone (Top-Level) JSON Values

JSON texts are allowed to be *any* value, not only an object/array:

```cpp
import std;
import xson;

using namespace std::string_literals;
using namespace xson;

auto v1 = json::parse("true");
auto v2 = json::parse("42");
auto v3 = json::parse("\"hello\"");

std::clog << json::stringify(v1) << "\n"; // true
std::clog << json::stringify(v2) << "\n"; // 42
std::clog << json::stringify(v3) << "\n"; // "hello"
```

## Behavior (highlights)

- **Standalone values**: a JSON text can be a value (not only object/array).
- **No trailing garbage**: any non-whitespace after a complete JSON text is rejected.
- **Strings**: rejects unescaped control chars; supports `\\uXXXX` including surrogate pairs.
- **Numbers**: rejects leading zeros; requires at least one digit after `.`; supports scientific notation.
- **Duplicate keys**: in objects, **last value wins**.

### Numbers

- **Within `int64_t` range**: stored as `integer_type`
- **Out of range**: stored as `number_type` (double)
- **Scientific notation**: supported (e.g. `1e2`, `1e-2`, `1.5e+2`)

```cpp
auto json_str = R"({"small": 42, "large": 9223372036854775808})";
auto ob = json::parse(json_str);

ob["small"s].is_integer();  // true - stored as int64_t
ob["large"s].is_integer();  // false - stored as double
ob["large"s].is_number();   // true - stored as double
```

## Module structure

The library is organized as C++23 modules:

- `xson` - main module
- `xson:json` - JSON parse/stringify (+ iostream operators)
- `xson:fson` - FSON binary serialization
- `xson:object` - object/array/value + builder
- `xson:fast` - fast utilities

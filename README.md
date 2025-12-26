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

## Usage

### JSON Object Creation

```cpp
import std;
import xson;

using namespace std::string_literals;
using namespace xson;

auto document = object{};

document["papa"s]["name"s] = "Cool"s;
document["papa"s]["age"s] = 40;
document["papa"s]["married"s] = false;
document["papa"s]["kids"s][1] = { "Name"s, "Tulppu"s };
document["papa"s]["kids"s][2] = { "Name"s, "Elppu"s };
document["papa"s]["kids"s][3] = { "Name"s, "Jalppu"s };

std::clog << json::stringify(document) << std::endl;
```

### JSON Parsing

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
// Note: requires `using xson::json::operator >>;`
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

## Module Structure

The library is organized as C++23 modules:

- `xson` - Main module exporting core types and functions
- `xson:json` - JSON parsing and stringification
- `xson:fson` - FSON (Fast JSON) binary serialization
- `xson:object` - Core object, array, and builder types
- `xson:fast` - Fast encoding/decoding utilities

## Features

- **C++23 Modules**: Fast compilation with module interfaces
- **JSON Support**: RFC 8259 style JSON parsing and stringification with scientific notation
- **FSON Support**: Binary serialization format for efficient storage
- **Type-Safe**: Strong typing with `integer_type`, `number_type`, `string_type`, etc.
- **Extensible**: Support for custom types via timestamp and integer types
- **Large Number Support**: Automatic handling of numbers exceeding `int64_t` range
- **Scientific Notation**: Full support for JSON scientific notation (e.g., `1e2`, `1.5e-3`, `1e+10`)

## Number Parsing Behavior

The JSON parser handles numbers intelligently based on their magnitude:

- **Integers within range**: Numbers from `INT64_MIN` (-9223372036854775808) to `INT64_MAX` (9223372036854775807) are stored as `integer_type` (`std::int64_t`)
- **Large integers**: Numbers exceeding `INT64_MAX` are automatically stored as `number_type` (`std::double_t`) to preserve precision
- **Type checking**: Use `is_integer()` to check if a value is stored as an integer, or `is_number()` to check if it's stored as a number (includes both integers and floats)

## JSON Parsing Strictness (RFC 8259 oriented)

- **Standalone JSON values**: A JSON text can be a value (not only object/array).
- **No trailing garbage**: Any non-whitespace after a complete JSON text is rejected.
- **Strings**:
  - Rejects **unescaped control characters** (U+0000..U+001F).
  - Supports `\\uXXXX` escapes including **surrogate pairs**.
- **Numbers**:
  - Rejects **leading zeros** in the integer part (e.g. `01`, `-01`).
  - Requires at least one digit after `.` (e.g. `2.` is invalid).
- **Duplicate object keys**: If the same key appears multiple times, the **last value wins**.

**Example:**
```cpp
auto json_str = R"({"small": 42, "large": 9223372036854775808})";
auto ob = json::parse(json_str);

ob["small"s].is_integer();  // true - stored as int64_t
ob["large"s].is_integer();  // false - stored as double
ob["large"s].is_number();   // true - stored as double
```

## Scientific Notation Support

The JSON parser fully supports scientific notation as specified in the JSON standard:

- **Positive exponents**: `1e2`, `1e+2` → 100.0
- **Negative exponents**: `1e-2`, `0.5e-3` → 0.01, 0.0005
- **Decimal numbers**: `1.5e2`, `3.14159e+0` → 150.0, 3.14159
- **Large/small numbers**: `1e308`, `1e-323` → Handles near double limits

**Example:**
```cpp
auto json_str = R"({"normal": 123, "scientific": 1e2, "negative": 1e-2})";
auto ob = json::parse(json_str);

ob["normal"s].is_integer();      // true - stored as int64_t
ob["scientific"s].is_number();   // true - stored as double (100.0)
ob["negative"s].is_number();     // true - stored as double (0.01)
```

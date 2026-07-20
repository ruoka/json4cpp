# json4cpp Assessment (2026)

**Date:** 2026-07-20  
**Repository:** https://github.com/ruoka/json4cpp  
**Reviewed at:** `90a46e0` (master)  
**Module:** `xson` (C++23 modules only)

Supersedes the 2024 peer review in [`archive/peer_review_2024.md`](archive/peer_review_2024.md).

---

## Verdict

Solid, production-usable JSON + FSON stack for CB/Clang 21 consumers (including YarDB). Parser correctness and `object::match` have improved materially since 2024. Main remaining gaps: **`max_array_size` is declared but never enforced**, and there is still **no `std::expected` parse API**, fuzzing, or FSON DoS limits. JSON stringify escaping for strings/keys is implemented (`write_json_string`).

---

## Architecture

| Module | File | Role |
|--------|------|------|
| `xson` | `xson.c++m` | Umbrella re-export |
| `xson:object` | `xson-object.c++m` | Value model, builder, `match`, primitive I/O |
| `xson:json` | `xson-json.c++m` | JSON decode / stringify / stream ops |
| `xson:fson` | `xson-fson.c++m` | Binary FSON encode/decode |
| `xson:fast` | `xson-fast.c++m` | Varint + stop-bit string codec |

**Value model:** `object` holds `variant<map, array, primitive>` with
`map = flat_map<string, object>`, `array = vector<object>`, and
`primitive = variant<monostate, double, string, time_point, int64, bool>`.

**Primary consumers:** `json::parse` / `stringify`, FSON round-trip for storage, `object::match` for selector queries (`$eq` / `$ne` / `$lt` / … / `$in` / `$nin`; `$top` / `$skip` / `$orderby` ignored for matching).

---

## What is solid

- **RFC 8259-oriented parse:** standalone values, no trailing garbage, leading-zero reject, fraction/exponent rules, unescaped controls rejected, `\uXXXX` + surrogate pairs → UTF-8.
- **Number policy:** in-range integers stay `int64`; overflow / scientific notation become `double` with exponent/finite checks.
- **DoS limits (JSON, partial):** `max_string_length` (100 MB) and `max_nesting_depth` (1000) are enforced; exponent digit caps exist.
- **Recent correctness:** int64 ↔ double numeric compare + rank for `match`; `$nin`; scalar operators rejected on object/array; FAST UTF-8 escape/unescape; full int64/uint64 varint range (`90a46e0`).
- **Tests:** ~10 co-located `*.test.c++`, ~252 `test_case` registrations, tag `[xson]`; strong RFC8259-named suite.
- **Build/CI:** CB + tester; Clang/LLVM **21** required (README + `.github/actions/setup-llvm`).

---

## Gaps (priority)

### High

1. **Enforce `max_array_size`** — constant is defined (`100 * 1024 * 1024` elements) but unused; only mentioned in a test comment. Wire it into array append paths in the JSON decoder; consider `max_object_members` too.
2. **`std::expected` parse path** — all errors throw `runtime_error` / `logic_error`. A `try_parse` (or equivalent) beside throwing `parse` would suit HTTP/server callers.

### Medium

3. **FSON DoS limits** — no nesting/string/array caps in `xson-fson.c++m` (storage path can still be abused if untrusted bytes are accepted).
4. **Fuzz JSON (and FSON) decode** — no libFuzzer / OSS-Fuzz targets; state machine is a natural target.
5. **RFC8259 suite stubs** — several invalid-UTF-8 / BOM `n_*` cases are empty skips; fill with raw-byte fixtures.
6. **README refresh** — still shows an old `CB.sh <std.cppm> …` snippet; document limits, FSON, `match`, and escape guarantees.
7. **`[[fallthrough]]`** on intentional varint switches in `xson-fast.c++m`.

### Low

8. Microbenchmarks (JSON vs FSON throughput).
9. Packaging / consumer notes (still CB-submodule only).
10. Document int64↔double compare precision (equality via `double` cast; large ints beyond mantissa).
11. Typed parse errors (offset + kind) instead of string-only exceptions.
12. Repo hygiene: root artifacts like `all_tests_output.txt` / empty `test_list.txt` should not live on `master`.

### Done (since this assessment)

- **JSON stringify escaping** — `write_json_string` escapes values and keys (`\"`, `\\`, `\b`/`\f`/`\n`/`\r`/`\t`, other controls as `\u00XX`); UTF-8 passed through.

---

## Limits reference (`xson-json.c++m`)

| Constant | Value | Enforced? |
|----------|-------|-----------|
| `max_string_length` | 100 MB | Yes |
| `max_nesting_depth` | 1000 | Yes |
| `max_array_size` | 100 M elements | **No** |
| `max_exponent` / `min_exponent` | ±308 | Yes (parse path) |

FSON: no corresponding limits.

---

## Suggested next work

1. Enforce `max_array_size` (+ object member cap).  
2. Add `try_parse` → `std::expected<object, parse_error>`.  
3. Mirror JSON limits in FSON decode.  
4. Fuzz harness + README/CI case.

---

## Consumers

YarDB pins this repo as `deps/xson`. Storage and OData-style filtering depend on FSON + `match`; HTTP bodies use JSON parse/stringify. **Array size enforcement** is the remaining high-priority item most likely to bite callers before packaging or benchmarks matter.

Historical 2024 review: [`archive/peer_review_2024.md`](archive/peer_review_2024.md).

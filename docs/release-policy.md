# Release Policy

## Current status: v3.2.0

**[`v3.2.0`](https://github.com/ruoka/json4cpp/releases/tag/v3.2.0)** (15 November 2026)
adds nested `try_get`, `try_string_view` and `try_object`, and `operator string_view`
for a string value, on the Clang 21 + libc++ modules consume surface (`import xson;`).
A missing field returns `nullopt`. A present field of the wrong type still throws.
`operator string_view` reads a string value without copying it. Object move construction
and move assignment are `noexcept`.
[`v3.1.0`](https://github.com/ruoka/json4cpp/releases/tag/v3.1.0) (23 October 2026)
added `object::try_get` and `object::value_or`. A missing field returns `nullopt` or
the caller-supplied default.
[`v3.0.0`](https://github.com/ruoka/json4cpp/releases/tag/v3.0.0) (1 August 2026) is the
first SemVer tag for that surface. Prior tags `v1.0` / `v2.0` are historical (header /
earlier C++23 lines).

Pin `v3.2.0` (or a later tag / deliberate commit). Nested `deps/tester` must match the
parent tree’s tester pin — see
[YarDB versioning](https://github.com/ruoka/YarDB/blob/master/docs/versioning.md).

## What is public

| Surface | Contract |
|---------|----------|
| `import xson;` | JSON/FSON parse and stringify, object/array builders, and related partitions documented in the [README](../README.md) and [docs/assessment.md](assessment.md) |
| Build / test | `./tools/CB.sh` with project tag `[xson]` |

Internal helpers and nested tester contents (beyond the gitlink SHA) are not part of the
json4cpp SemVer contract.

## Versioning

Semantic versioning (`vMAJOR.MINOR.PATCH`) on the public surface above.

| Bump | Means |
|------|-------|
| MAJOR | Incompatible change to the exported module API, or raising the minimum toolchain |
| MINOR | Additive, backward-compatible public change |
| PATCH | Fix that leaves documented behaviour intact |

**Modules-era major.** `v3.0.0` is the modules consume-surface line relative to `v1`/`v2`.

**Tester pin.** When bumping `deps/tester`, land the same SHA in cryptic, net4cpp,
json4cpp, and YarDB together.

**No bump required.** Pin-only `deps/tester` syncs and test-only / docs-only /
CI-only changes that leave the public surface above unchanged do **not** require a
new SemVer tag. See
[YarDB versioning — When not to bump](https://github.com/ruoka/YarDB/blob/master/docs/versioning.md#when-not-to-bump).

## Release criteria

1. Default-branch CI green (`debug`/`release`, `--jsonl=failures`, `--jobs=$(nproc)`,
   `--tags='\[xson\]'`).
2. Open PRs that affect the release surface are merged or closed.
3. Nested `deps/tester` matches the ecosystem pin rule.
4. README / this policy agree with the tagged surface; minimum toolchain stated.

## Cutting a release

```bash
./tools/CB.sh debug test --jsonl=failures --jobs="$(nproc)" --tags='\[xson\]'
git tag -a vX.Y.Z -m "vX.Y.Z"
git push origin vX.Y.Z
gh release create vX.Y.Z --title vX.Y.Z --notes-file release-notes.md
```

Release notes: breaking changes, additions, fixes, minimum compiler, tester pin SHA/tag.

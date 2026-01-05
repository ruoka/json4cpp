#!/usr/bin/env bash
# deps/xson/tools/CB.sh — C++ Builder bootstrap for xson submodule
# - Prefers a sibling tester (e.g. when xson is used inside a larger repo with deps/tester)
# - Falls back to local deps/tester (for standalone xson repo)
# - Optionally clones tester from GitHub if missing (export CB_FETCH_DEPS=1)

set -e

TOOLS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOLS_DIR/.." && pwd)"
BIN="$TOOLS_DIR/cb"

# Default behavior:
# - On normal dev machines: run all tests by default.
# - In Cursor sandbox/CI-like environments: disable network tests unless user explicitly overrides.
if [[ -n "${CURSOR_SANDBOX:-}" && -z "${NET_DISABLE_NETWORK_TESTS+x}" ]]; then
  export NET_DISABLE_NETWORK_TESTS=1
fi

# Resolve tester root:
# 1) sibling deps/tester (common when PROJECT_ROOT is .../deps/xson)
# 2) local deps/tester (standalone xson repo)
TESTER_ROOT=""
if [[ -d "$PROJECT_ROOT/../tester/tools" ]]; then
  TESTER_ROOT="$(cd "$PROJECT_ROOT/../tester" && pwd)"
elif [[ -d "$PROJECT_ROOT/deps/tester/tools" ]]; then
  TESTER_ROOT="$(cd "$PROJECT_ROOT/deps/tester" && pwd)"
fi

if [[ -z "$TESTER_ROOT" ]]; then
  if [[ "${CB_FETCH_DEPS:-0}" == "1" ]]; then
    mkdir -p "$PROJECT_ROOT/deps"
    echo "tester dependency missing; cloning into '$PROJECT_ROOT/deps/tester'..."
    git clone --depth 1 "https://github.com/ruoka/tester.git" "$PROJECT_ROOT/deps/tester"
    TESTER_ROOT="$(cd "$PROJECT_ROOT/deps/tester" && pwd)"
  else
    echo "ERROR: tester dependency not found."
    echo "Looked for:"
    echo "  - $PROJECT_ROOT/../tester"
    echo "  - $PROJECT_ROOT/deps/tester"
    echo
    echo "Fix by initializing submodules or cloning tester:"
    echo "  git submodule update --init --recursive"
    echo "or rerun with:"
    echo "  CB_FETCH_DEPS=1 $0 $*"
    exit 1
  fi
fi

SRC="$TESTER_ROOT/tools/cb.c++"

# Detect OS and set compiler/LLVM paths
UNAME_OUT="$(uname -s)"
case "$UNAME_OUT" in
    Linux)
    CXX_COMPILER="clang++-21"
    LLVM_PREFIX="/usr/lib/llvm-21"
        STD_CPPM_DEFAULT="/usr/lib/llvm-21/share/libc++/v1/std.cppm"
        ;;
    Darwin)
    CXX_COMPILER="/usr/local/llvm/bin/clang++"
    LLVM_PREFIX="/usr/local/llvm"
        STD_CPPM_DEFAULT="/usr/local/llvm/share/libc++/v1/std.cppm"
        ;;
    *)
    echo "ERROR: Unsupported OS '$UNAME_OUT'"
    exit 1
        ;;
esac

# Ensure we always search the LLVM lib dir when linking CB itself
export LDFLAGS="-Wl,-rpath,$LLVM_PREFIX/lib ${LDFLAGS}"

# Check if binary exists and was built for the correct OS
NEEDS_REBUILD=false
if [[ ! -x "$BIN" ]]; then
    NEEDS_REBUILD=true
elif [[ "$SRC" -nt "$BIN" ]]; then
    NEEDS_REBUILD=true
else
    # Check binary format using file command (portable across Linux and macOS)
    if [[ -f "$BIN" ]] && command -v file >/dev/null 2>&1; then
        FILE_TYPE=$(file "$BIN" 2>/dev/null || echo "")
        case "$UNAME_OUT" in
            Linux)
                if [[ "$FILE_TYPE" != *"ELF"* ]]; then
                    echo "CB binary was built for a different OS (not Linux), rebuilding..."
                    NEEDS_REBUILD=true
                fi
                ;;
            Darwin)
                if [[ "$FILE_TYPE" != *"Mach-O"* ]]; then
                    echo "CB binary was built for a different OS (not macOS), rebuilding..."
                    NEEDS_REBUILD=true
                fi
                ;;
        esac
    fi
fi

# Rebuild if needed
if [[ "$NEEDS_REBUILD" == "true" ]]; then
    echo "Building CB (C++ Builder) with $CXX_COMPILER..."
    # Use -B to tell clang++ where to find binaries (like the linker)
    "$CXX_COMPILER" \
        -B"$LLVM_PREFIX/bin" \
        -std=c++23 -O3 -pthread \
        -fuse-ld=lld \
        -stdlib=libc++ \
        -I"$LLVM_PREFIX/include/c++/v1" \
        -L"$LLVM_PREFIX/lib" \
        -Wl,-rpath,"$LLVM_PREFIX/lib" \
        -lc++abi \
        "$SRC" -o "$BIN"
    echo "CB compiled successfully → $BIN"
fi

# Resolve std.cppm path: explicit argument (with slash or .cppm suffix) wins,
# otherwise use LLVM_PATH or defaults per OS.
STD_CPPM=""
if [[ -n "$1" && ("$1" == *.cppm || "$1" == */*) ]]; then
        STD_CPPM="$1"
        shift
fi

if [[ -z "$STD_CPPM" ]]; then
    STD_CPPM="${LLVM_PATH:-$STD_CPPM_DEFAULT}"
fi

if [[ ! -f "$STD_CPPM" ]]; then
    echo "ERROR: std.cppm not found at '$STD_CPPM'."
    echo "Pass the path as the first argument or set LLVM_PATH."
    exit 1
fi

# Detect xson project structure and build include flags
INCLUDE_FLAGS=(
  -I "$PROJECT_ROOT/xson"
)

# tester module include dir
if [[ -d "$TESTER_ROOT/tester" ]]; then
  INCLUDE_FLAGS+=(-I "$TESTER_ROOT/tester")
fi

# Homebrew headers (macOS) – harmless if unused
if [[ "$UNAME_OUT" == "Darwin" ]]; then
  INCLUDE_FLAGS+=(-I "/opt/homebrew/include")
fi

# Allow override via environment variable
if [[ -n "$CB_INCLUDE_FLAGS" ]]; then
    # Parse space-separated paths from environment variable
    INCLUDE_FLAGS=()
    for path in $CB_INCLUDE_FLAGS; do
        INCLUDE_FLAGS+=(-I "$path")
    done
fi

# Run it with resolved std.cppm path and include flags
exec "$BIN" "$STD_CPPM" "${INCLUDE_FLAGS[@]}" "$@"


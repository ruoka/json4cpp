#!/usr/bin/env bash
# tools/CB.sh — C++ Builder bootstrap for xson project
# Works perfectly with your /usr/local/llvm setup

set -e

TOOLS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOLS_DIR/.." && pwd)"
SRC="$PROJECT_ROOT/deps/tester/tools/cb.c++"
BIN="$TOOLS_DIR/cb"

# Detect OS and set compiler/LLVM paths
UNAME_OUT="$(uname -s)"
case "$UNAME_OUT" in
    Linux)
    CXX_COMPILER="clang++-20"
    LLVM_PREFIX="/usr/lib/llvm-20"
        STD_CPPM_DEFAULT="/usr/lib/llvm-20/share/libc++/v1/std.cppm"
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
# Get the current working directory (where cb will be invoked from)
CURRENT_DIR="$(pwd)"
INCLUDE_FLAGS=()

# Check if we're in the xson project root (has xson/ directory and deps/)
if [[ -d "$CURRENT_DIR/xson" && -d "$CURRENT_DIR/deps" ]]; then
    # xson project structure
    # Note: -I "$CURRENT_DIR/xson" is needed for module imports
    INCLUDE_FLAGS=(
        -I "$CURRENT_DIR/xson"
        -I "$CURRENT_DIR/deps/tester/tester"
        -I "/opt/homebrew/include"
    )
# Check if we're in a subdirectory of xson project
elif [[ "$CURRENT_DIR" == *"/xson"* ]] && [[ -d "$(cd "$CURRENT_DIR/../.." 2>/dev/null && pwd)/xson" ]]; then
    # We're in a subdirectory, find project root
    PROJECT_ROOT_FOUND=""
    CHECK_DIR="$CURRENT_DIR"
    while [[ "$CHECK_DIR" != "/" ]]; do
        if [[ -d "$CHECK_DIR/xson" && -d "$CHECK_DIR/deps" ]]; then
            PROJECT_ROOT_FOUND="$CHECK_DIR"
            break
        fi
        CHECK_DIR="$(cd "$CHECK_DIR/.." 2>/dev/null && pwd)"
    done
    if [[ -n "$PROJECT_ROOT_FOUND" ]]; then
        INCLUDE_FLAGS=(
            -I "$PROJECT_ROOT_FOUND/xson"
            -I "$PROJECT_ROOT_FOUND/deps/tester/tester"
            -I "/opt/homebrew/include"
        )
    fi
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


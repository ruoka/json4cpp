#!/usr/bin/env bash
# deps/xson/tools/CB.sh — xson bootstrap (library + tester)

set -e

TOOLS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOLS_DIR/.." && pwd)"

TESTER_ROOT=""
if [[ -d "$PROJECT_ROOT/../tester/tools" ]]; then
  TESTER_ROOT="$(cd "$PROJECT_ROOT/../tester" && pwd)"
elif [[ -d "$PROJECT_ROOT/deps/tester/tools" ]]; then
  TESTER_ROOT="$(cd "$PROJECT_ROOT/deps/tester" && pwd)"
fi

CB_TOOLS_DIR="$TOOLS_DIR"
CB_PROJECT_ROOT="$PROJECT_ROOT"
CB_TESTER_ROOT="$TESTER_ROOT"
CB_RESPECT_CXX_ENV=0

CB_INCLUDE_DIRS=("$PROJECT_ROOT/xson")
[[ -n "$TESTER_ROOT" && -d "$TESTER_ROOT/tester" ]] && CB_INCLUDE_DIRS+=("$TESTER_ROOT/tester")
if [[ "$(uname -s)" == "Darwin" && -d "/opt/homebrew/include" ]]; then
  CB_INCLUDE_DIRS+=("/opt/homebrew/include")
fi

CB_CORE=""
if [[ -n "$TESTER_ROOT" ]]; then
  CB_CORE="$TESTER_ROOT/tools/CB.sh.core"
elif [[ -f "$PROJECT_ROOT/deps/tester/tools/CB.sh.core" ]]; then
  CB_CORE="$PROJECT_ROOT/deps/tester/tools/CB.sh.core"
else
  CB_CORE="$PROJECT_ROOT/../tester/tools/CB.sh.core"
fi
source "$CB_CORE"
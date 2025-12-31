#!/usr/bin/env bash

set -euo pipefail
set -x

: "${CC:-clang}"
CFLAGS="$(pkg-config --libs --cflags liburing)"

"$CC" ./kmemleak.c $CFLAGS -o kmemleak
"$CC" ./memleak.c $CFLAGS -o memleak


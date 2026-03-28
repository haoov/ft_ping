#!/bin/bash
# ft_ping test suite — run as root (e.g. inside a container)
# Usage: bash test.sh

FT_PING="./ft_ping"
TARGET="8.8.8.8"     # reliable public host
UNREACH="192.0.2.1"  # RFC 5737 reserved, guaranteed unreachable

# ── colours ──────────────────────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
DIM='\033[2m'
RESET='\033[0m'

PASS=0; FAIL=0; SKIP=0

# ── helpers ───────────────────────────────────────────────────────────────────

section() {
    echo ""
    echo -e "${BOLD}${BLUE}┌──────────────────────────────────────────────────┐${RESET}"
    printf "${BOLD}${BLUE}│  %-48s│${RESET}\n" "$1"
    echo -e "${BOLD}${BLUE}└──────────────────────────────────────────────────┘${RESET}"
}

_ok()   { echo -e "${GREEN}PASS${RESET}"; ((PASS++)); }
_fail() {
    echo -e "${RED}FAIL${RESET}"
    [ -n "$1" ] && echo -e "    ${DIM}expected : $1${RESET}"
    [ -n "$2" ] && echo -e "    ${DIM}got      : $(echo "$2" | head -5)${RESET}"
    ((FAIL++))
}

label() { printf "  ${CYAN}%-52s${RESET}" "$1"; }

# run cmd, check output matches regex; want_fail=1 → expect non-zero exit
check() {
    local name="$1" cmd="$2" pattern="$3" want_fail="${4:-0}"
    label "$name"
    local out; out=$(eval "$cmd" 2>&1)
    local rc=$? matched=0
    echo "$out" | grep -qE -- "$pattern" && matched=1

    if [ "$want_fail" = "1" ]; then
        [[ $rc -ne 0 && $matched -eq 1 ]] && { _ok; return; }
    else
        [[ $rc -eq 0 && $matched -eq 1 ]] && { _ok; return; }
    fi
    _fail "$pattern" "$out"
}

# assert pattern is ABSENT in output
check_absent() {
    local name="$1" cmd="$2" pattern="$3"
    label "$name"
    local out; out=$(eval "$cmd" 2>&1)
    if echo "$out" | grep -qE -- "$pattern"; then
        _fail "should NOT contain '$pattern'" "$out"
    else
        _ok
    fi
}

# timing check: elapsed seconds must be in [min, max]
check_time() {
    local name="$1" cmd="$2" min="$3" max="$4"
    label "$name"
    local t0 t1 elapsed
    t0=$(date +%s%N)
    eval "$cmd" >/dev/null 2>&1
    t1=$(date +%s%N)
    elapsed=$(( (t1 - t0) / 1000000000 ))
    if [[ $elapsed -ge $min && $elapsed -le $max ]]; then
        echo -e "${GREEN}PASS${RESET}  ${DIM}(${elapsed}s, expected ${min}–${max}s)${RESET}"
        ((PASS++))
    else
        echo -e "${RED}FAIL${RESET}  ${DIM}(took ${elapsed}s, expected ${min}–${max}s)${RESET}"
        ((FAIL++))
    fi
}

# ── preflight ─────────────────────────────────────────────────────────────────
echo ""
echo -e "${BOLD}  ft_ping test suite${RESET}"
echo -e "  ${DIM}binary : $FT_PING${RESET}"
echo -e "  ${DIM}target : $TARGET${RESET}"
echo -e "  ${DIM}unreach: $UNREACH${RESET}"

if [ ! -f "$FT_PING" ]; then
    echo -e "\n${RED}  Error: $FT_PING not found — run 'make' first.${RESET}\n"
    exit 1
fi

# ═════════════════════════════════════════════════════════════════════════════
section "1 · ARGUMENT ERRORS"
# ═════════════════════════════════════════════════════════════════════════════

check "no arguments → missing host operand" \
    "$FT_PING" \
    "missing host operand" 1

check "unknown long option → error" \
    "$FT_PING --foobar" \
    "unrecognized option" 1

check "unknown short option → error" \
    "$FT_PING -Z" \
    "unrecognized option" 1

check "-c requires an argument" \
    "$FT_PING -c" \
    "requires an argument" 1

check "-i requires an argument" \
    "$FT_PING -i" \
    "requires an argument" 1

check "-s requires an argument" \
    "$FT_PING -s" \
    "requires an argument" 1

check "-c 0 → invalid value (zero)" \
    "$FT_PING -c 0 $TARGET" \
    "invalid value" 1

check "-c -1 → invalid value (negative)" \
    "$FT_PING -c -1 $TARGET" \
    "invalid value" 1

check "-s 0 → invalid value" \
    "$FT_PING -s 0 $TARGET" \
    "invalid value" 1

check "-i 0 → invalid value (zero interval)" \
    "$FT_PING -i 0 $TARGET" \
    "invalid value" 1

check "-W 0 → invalid value (zero linger)" \
    "$FT_PING -W 0 $TARGET" \
    "invalid value" 1

check "unknown hostname → gai error" \
    "$FT_PING thisdomaindoesnotexist.invalid" \
    "thisdomaindoesnotexist.invalid" 1

# ═════════════════════════════════════════════════════════════════════════════
section "2 · HELP / VERSION"
# ═════════════════════════════════════════════════════════════════════════════

check "--help prints usage" \
    "$FT_PING --help" \
    "Usage:"

check "-? prints usage" \
    "$FT_PING '-?'" \
    "Usage:"

check "--version prints version string" \
    "$FT_PING --version" \
    "version"

# ═════════════════════════════════════════════════════════════════════════════
section "3 · OUTPUT FORMAT"
# ═════════════════════════════════════════════════════════════════════════════

check "PING header: colon + data(total) notation" \
    "$FT_PING -c 1 $TARGET" \
    "PING $TARGET \($TARGET\): 56\(84\) bytes of data\."

check "per-packet line format" \
    "$FT_PING -c 1 $TARGET" \
    "[0-9]+ bytes from $TARGET: icmp_seq=[0-9]+ ttl=[0-9]+ time=[0-9]+\.[0-9]+ ms"

check "stats separator line" \
    "$FT_PING -c 1 $TARGET" \
    "--- $TARGET ping statistics ---"

check "packet loss line format" \
    "$FT_PING -c 1 $TARGET" \
    "[0-9]+ packets transmitted, [0-9]+ packets received, [0-9]+% packet loss"

check "RTT line format" \
    "$FT_PING -c 1 $TARGET" \
    "rtt min/avg/max/mdev = [0-9]+\.[0-9]+/[0-9]+\.[0-9]+/[0-9]+\.[0-9]+/[0-9]+\.[0-9]+ ms"

check "reply IP matches target (not local)" \
    "$FT_PING -c 1 $TARGET" \
    "bytes from $TARGET:"

# ═════════════════════════════════════════════════════════════════════════════
section "4 · COUNT  (-c)"
# ═════════════════════════════════════════════════════════════════════════════

check "-c 1 → exactly 1 packet transmitted" \
    "$FT_PING -c 1 $TARGET" \
    "^1 packets transmitted"

check "-c 3 → exactly 3 packets transmitted" \
    "$FT_PING -c 3 $TARGET" \
    "^3 packets transmitted"

check "-c 3 → 0% packet loss on reachable host" \
    "$FT_PING -c 3 $TARGET" \
    "3 packets transmitted, 3 packets received, 0% packet loss"

# ═════════════════════════════════════════════════════════════════════════════
section "5 · QUIET  (-q)"
# ═════════════════════════════════════════════════════════════════════════════

check "-q still prints PING header" \
    "$FT_PING -c 3 -q $TARGET" \
    "PING.*bytes of data"

check "-q still prints stats" \
    "$FT_PING -c 3 -q $TARGET" \
    "packets transmitted"

check_absent "-q suppresses per-packet lines" \
    "$FT_PING -c 3 -q $TARGET" \
    "bytes from"

# ═════════════════════════════════════════════════════════════════════════════
section "6 · SIZE  (-s)"
# ═════════════════════════════════════════════════════════════════════════════

check "-s 100 → header shows 100(128)" \
    "$FT_PING -c 1 -s 100 $TARGET" \
    "100\(128\) bytes of data"

check "-s 100 → packet line shows 108 bytes (100+icmphdr)" \
    "$FT_PING -c 1 -s 100 $TARGET" \
    "108 bytes from $TARGET:"

# ═════════════════════════════════════════════════════════════════════════════
section "7 · INTERVAL  (-i)"
# ═════════════════════════════════════════════════════════════════════════════

check_time "-i 2 with 3 packets → takes 4–8s" \
    "$FT_PING -c 3 -i 2 $TARGET" 4 8

check_time "-i 1 (default) with 3 packets → takes 2–5s" \
    "$FT_PING -c 3 $TARGET" 2 5

# ═════════════════════════════════════════════════════════════════════════════
section "8 · LINGER / TIMEOUT  (-W)"
# ═════════════════════════════════════════════════════════════════════════════

check "-W 2 on unreachable IP → request timeout message" \
    "$FT_PING -c 1 -W 2 $UNREACH" \
    "Request timeout"

check_time "-W 2 on unreachable IP → exits in 1–5s" \
    "$FT_PING -c 1 -W 2 $UNREACH" 1 5

check "-W 2 on unreachable → 100% packet loss" \
    "$FT_PING -c 2 -W 2 $UNREACH" \
    "100% packet loss"

# ═════════════════════════════════════════════════════════════════════════════
section "9 · TTL + VERBOSE  (--ttl / -v)"
# ═════════════════════════════════════════════════════════════════════════════

check_absent "--ttl 1 without -v → ICMP errors are silent" \
    "$FT_PING -c 2 --ttl 1 $TARGET" \
    "[Ee]xceeded|[Uu]nreachable"

check "--ttl 1 -v → prints Time to live exceeded" \
    "$FT_PING -c 2 -v --ttl 1 $TARGET" \
    "[Tt]ime to live exceeded"

check "--ttl 1 -v → still prints stats" \
    "$FT_PING -c 2 -v --ttl 1 $TARGET" \
    "packets transmitted"

# ═════════════════════════════════════════════════════════════════════════════
section "10 · PATTERN  (-p)"
# ═════════════════════════════════════════════════════════════════════════════

check "-p deadbeef → ping completes successfully" \
    "$FT_PING -c 1 -p deadbeef $TARGET" \
    "bytes from $TARGET:"

check "-p ff → 1 packet transmitted" \
    "$FT_PING -c 1 -p ff $TARGET" \
    "1 packets transmitted"

# ═════════════════════════════════════════════════════════════════════════════
section "11 · NUMERIC  (-n)"
# ═════════════════════════════════════════════════════════════════════════════

check "-n with IP → pings successfully" \
    "$FT_PING -c 1 -n $TARGET" \
    "bytes from $TARGET:"

# ═════════════════════════════════════════════════════════════════════════════
section "12 · STATS ACCURACY"
# ═════════════════════════════════════════════════════════════════════════════

check "rtt line present after 5 packets" \
    "$FT_PING -c 5 $TARGET" \
    "rtt min/avg/max/mdev"

label "mdev = 0.000 for single packet"
out=$($FT_PING -c 1 $TARGET 2>&1)
# RTT line: "rtt min/avg/max/mdev = X/X/X/X ms" — extract 4th slash-separated value
mdev=$(echo "$out" | grep "^rtt" | grep -oE "[0-9]+\.[0-9]+/[0-9]+\.[0-9]+/[0-9]+\.[0-9]+/[0-9]+\.[0-9]+" | cut -d'/' -f4)
if [ "$mdev" = "0.000" ]; then
    _ok
else
    _fail "mdev=0.000" "mdev=$mdev  full output: $out"
fi

label "no rtt line when 0 packets received"
out=$($FT_PING -c 1 -W 2 $UNREACH 2>&1)
if echo "$out" | grep -q "rtt min"; then
    _fail "should not print rtt when nrecv=0" "$out"
else
    _ok
fi

# ═════════════════════════════════════════════════════════════════════════════
section "13 · FQDN"
# ═════════════════════════════════════════════════════════════════════════════

check "PING header shows resolved IP for FQDN" \
    "$FT_PING -c 1 google.com" \
    "PING google\.com \([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\): [0-9]+\([0-9]+\) bytes"

check "packet line shows numeric IP (not hostname)" \
    "$FT_PING -c 1 google.com" \
    "bytes from [0-9]+\.[0-9]+\.[0-9]+\.[0-9]+:"

check_absent "packet line does NOT show 'google.com'" \
    "$FT_PING -c 1 google.com" \
    "bytes from google"

# ═════════════════════════════════════════════════════════════════════════════
section "14 · SIGINT → stats printed"
# ═════════════════════════════════════════════════════════════════════════════

label "SIGINT after 2s → stats separator present"
tmpfile=$(mktemp)
$FT_PING $TARGET > "$tmpfile" 2>&1 &
PID=$!
sleep 2
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
if grep -q "ping statistics" "$tmpfile"; then
    _ok
else
    _fail "ping statistics block" "$(cat $tmpfile)"
fi

label "SIGINT after 2s → packet loss line present"
if grep -q "packets transmitted" "$tmpfile"; then
    _ok
else
    _fail "packets transmitted line" "$(cat $tmpfile)"
fi
rm -f "$tmpfile"

# ═════════════════════════════════════════════════════════════════════════════
section "15 · MULTIPLE HOSTS"
# ═════════════════════════════════════════════════════════════════════════════

label "two hosts → two stats blocks"
out=$($FT_PING -c 1 $TARGET 1.1.1.1 2>&1)
count=$(echo "$out" | grep -c "ping statistics")
if [ "$count" = "2" ]; then
    _ok
else
    _fail "2 stats blocks" "found $count  output: $out"
fi

check "two hosts → first host stats present" \
    "$FT_PING -c 1 $TARGET 1.1.1.1" \
    "$TARGET ping statistics"

check "two hosts → second host stats present" \
    "$FT_PING -c 1 $TARGET 1.1.1.1" \
    "1\.1\.1\.1 ping statistics"

# ═════════════════════════════════════════════════════════════════════════════
section "16 · RTT ORDERING  (min ≤ avg ≤ max)"
# ═════════════════════════════════════════════════════════════════════════════

label "min ≤ avg ≤ max over 5 packets"
out=$($FT_PING -c 5 $TARGET 2>&1)
rtt_vals=$(echo "$out" | grep "^rtt" | grep -oE "[0-9]+\.[0-9]+/[0-9]+\.[0-9]+/[0-9]+\.[0-9]+")
if echo "$rtt_vals" | awk -F'/' '{ if ($1+0 <= $2+0 && $2+0 <= $3+0) exit 0; else exit 1 }'; then
    _ok
else
    _fail "min <= avg <= max" "rtt values: $rtt_vals"
fi

# ═════════════════════════════════════════════════════════════════════════════
section "17 · VERBOSE ALONE  (-v)"
# ═════════════════════════════════════════════════════════════════════════════

check "-v accepted, normal packet line still printed" \
    "$FT_PING -v -c 1 $TARGET" \
    "bytes from $TARGET:"

check "-v -q combo: -q wins, no per-packet output" \
    "$FT_PING -v -q -c 1 $TARGET" \
    "packets transmitted"

check_absent "-v -q: still no per-packet lines" \
    "$FT_PING -v -q -c 1 $TARGET" \
    "bytes from"

# ═════════════════════════════════════════════════════════════════════════════
section "18 · TOTAL DEADLINE  (-w)"
# ═════════════════════════════════════════════════════════════════════════════

check_time "-w 3 without -c → stops after 3–7s" \
    "$FT_PING -w 3 $TARGET" 3 7

check "-w 3 with -c 100 → far fewer than 100 packets sent" \
    "$FT_PING -w 3 -c 100 $TARGET" \
    "^[1-9] packets transmitted"

check_time "-w 5 -W 2 on unreachable → stops after 4–10s" \
    "$FT_PING -w 5 -W 2 $UNREACH" 4 10

check "-w 3 → stats still printed" \
    "$FT_PING -w 3 $TARGET" \
    "ping statistics"

# ═════════════════════════════════════════════════════════════════════════════
# SUMMARY
# ═════════════════════════════════════════════════════════════════════════════
TOTAL=$((PASS + FAIL + SKIP))
echo ""
echo -e "${BOLD}${BLUE}┌──────────────────────────────────────────────────┐${RESET}"
printf "${BOLD}${BLUE}│  %-48s│${RESET}\n" "RESULTS"
echo -e "${BOLD}${BLUE}├──────────────────────────────────────────────────┤${RESET}"
printf "  ${GREEN}${BOLD}%-4s passed${RESET}   ${RED}${BOLD}%-4s failed${RESET}   ${YELLOW}${BOLD}%-4s skipped${RESET}   / %s total\n" \
    "$PASS" "$FAIL" "$SKIP" "$TOTAL"
echo -e "${BOLD}${BLUE}└──────────────────────────────────────────────────┘${RESET}"
echo ""

[ $FAIL -gt 0 ] && exit 1
exit 0

# ft_ping

A from-scratch reimplementation of the `ping` command in C, based on the
[inetutils-2.0](https://www.gnu.org/software/inetutils/) reference.
Built as part of the 42 school network projects track.

---

## How it works

`ping` tests the reachability of a host over an IP network by sending **ICMP
Echo Request** packets and waiting for **ICMP Echo Reply** responses.
Each round trip is timed to measure network latency.

```
ft_ping google.com

PING google.com (142.250.185.238): 56(84) bytes of data.
64 bytes from 142.250.185.238: icmp_seq=1 ttl=117 time=4.438 ms
64 bytes from 142.250.185.238: icmp_seq=2 ttl=117 time=4.201 ms
^C
--- google.com ping statistics ---
2 packets transmitted, 2 packets received, 0% packet loss
rtt min/avg/max/mdev = 4.201/4.319/4.438/0.118 ms
```

Because it uses **raw sockets**, the binary must be run as **root**.

---

## Build

```bash
make          # produces ./ft_ping
make re       # clean rebuild
make clean    # remove binary
```

Requirements: `gcc`, `make`, `libm` (math). Tested on Debian (kernel ≥ 3.14).

---

## Usage

```
ft_ping [options] <host> [<host2> ...]
```

Up to 16 hosts can be specified; each is pinged sequentially with its own
statistics block.

### Options

| Flag | Long form | Default | Description |
|------|-----------|---------|-------------|
| `-c N` | `--count=N` | ∞ | Stop after sending N packets |
| `-i N` | `--interval=N` | `1.0` | Wait N seconds between packets |
| `-w N` | `--timeout=N` | off | Total deadline in seconds (stops regardless of `-c`) |
| `-W N` | `--linger=N` | `10.0` | Per-packet receive timeout in seconds |
| `-s N` | `--size=N` | `56` | Payload size in bytes |
| `-p PAT` | `--pattern=PAT` | — | Fill payload with hex pattern (e.g. `deadbeef`) |
| `-q` | `--quiet` | off | Suppress per-packet output; show only summary |
| `-v` | `--verbose` | off | Show ICMP error responses (TTL exceeded, unreachable…) |
| `-n` | `--numeric` | off | Treat host as a numeric IP (skip DNS lookup) |
| `-T N` | `--tos=N` | `0` | Set IP Type of Service byte |
| `--ttl=N` | | `64` | Set IP Time To Live |
| `-?` | `--help` | | Print usage and exit |
| `-V` | `--version` | | Print version and exit |

---

## Examples

```bash
# Basic ping — runs until Ctrl+C
ft_ping 8.8.8.8

# Send exactly 5 packets, 2-second interval
ft_ping -c 5 -i 2 google.com

# Quiet mode — only show the summary
ft_ping -c 10 -q 1.1.1.1

# Stop after 5 seconds total, even if -c not reached
ft_ping -w 5 google.com

# Force a TTL-exceeded error, show it with -v
ft_ping -v --ttl 1 8.8.8.8

# Custom payload pattern
ft_ping -c 3 -p deadbeef google.com

# Larger packet (100 bytes data = 128 bytes total)
ft_ping -c 3 -s 100 8.8.8.8

# Ping two hosts sequentially
ft_ping -c 3 8.8.8.8 1.1.1.1
```

---

## Project structure

```
ft_ping/
├── inc/
│   └── ft_ping.h       # structs, enums, all function prototypes
├── src/
│   ├── main.c          # entry point, option table, signal setup
│   ├── arg.c           # argument parser, option validation
│   ├── init.c          # socket creation, buffer allocation, TTL/timeout setup
│   ├── ping.c          # main ping loop (-w deadline, -i interval, -c count)
│   ├── request.c       # ICMP echo request construction and send
│   ├── response.c      # ICMP response parsing (echo reply + error types)
│   ├── stats.c         # RTT statistics (min/avg/max/mdev)
│   ├── utils.c         # hostname resolution, checksum, help text
│   ├── signal.c        # SIGINT handler
│   ├── print.c         # ping_error (formatted error + exit)
│   └── gc.c            # simple garbage collector (malloc tracking + atexit cleanup)
├── Dockerfile          # Debian image with inetutils-2.0 for reference comparison
├── test.sh             # automated test suite (57 tests across 18 sections)
└── Makefile
```

---

## Architecture notes

**Option table** (`main.c`) — all supported flags live in a single `struct opt`
array with name, short char, type (`boolean` / `number` / `floating` / `string`)
and default value. The parser (`arg.c`) handles both `-x val` and `--long=val`
forms and validates values before any socket is opened.

**Raw socket** — one `SOCK_RAW / IPPROTO_ICMP` socket is created per run.
`SO_RCVTIMEO` is set to the `-W` linger value so `recvfrom` times out cleanly
per packet. `IP_TTL` is set from `--ttl`.

**Timing** — `gettimeofday` is called immediately before `sendto` and immediately
after `recvfrom`. RTT is stored per-sequence in a circular timing table
(`stats.timing[seq % MAX_TIMING_PKT]`), allowing out-of-order replies to be
matched correctly.

**Statistics** — `mdev` (mean deviation) is computed from the running sum and
sum-of-squares: `mdev = sqrt(Σx²/n − (Σx/n)²)`, matching inetutils behaviour.

**Garbage collector** — `gc_add` registers every `malloc`'d pointer. `ping_exit`
(registered via `atexit`) walks the list and frees everything, so all error paths
are leak-free without manual cleanup at each call site.

---

## Running inside Docker

The Dockerfile builds a Debian image with inetutils-2.0 installed, useful for
direct output comparison:

```bash
# Build image and copy project in
docker build -t ft_ping .
docker run --rm -it --cap-add=NET_RAW ft_ping bash

# Inside container
cd /ft_ping && make
./ft_ping -c 3 8.8.8.8

# Compare with reference
ping -c 3 8.8.8.8
```

---

## Tests

```bash
bash test.sh
```

The suite covers 18 sections: argument errors, help/version, output format,
count, quiet, size, interval, linger, TTL + verbose, pattern, numeric,
stats accuracy, FQDN, SIGINT, multiple hosts, RTT ordering, verbose alone,
and the total deadline (`-w`).

---

## Compliance

Output format (header line, per-packet line, statistics block) matches
inetutils-2.0 exactly, as required by the subject — except the RTT line
format and reverse-DNS resolution, which are explicitly excluded.

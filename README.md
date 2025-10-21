# SyStats

Small command-line tool written in C to display simple system statistics (memory, CPU model and load) using /proc on Linux.

## Overview
- `sys_stats` — reads `/proc/meminfo`, `/proc/cpuinfo` and `/proc/loadavg` and prints gauges and summary info.
  - See source: [`sys_stats.c`](sys_stats.c) (functions: [`get_memory_info`](sys_stats.c), [`get_cpu_info`](sys_stats.c), [`get_load_avg`](sys_stats.c), [`print_gauge`](sys_stats.c)).

## Build
Run from the project directory:
```bash
gcc sys_stats.c -o sys_stats
```

## Usage
```bash
./sys_stats
```
Reads the kernel procfs, so run on Linux systems with `/proc` mounted.

## Notes
- `sys_stats` uses ANSI colors and Unicode block characters for gauges; run in a UTF-8 capable terminal.


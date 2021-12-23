#!/bin/env zsh

time=$(date +%T.%N)
echo "[${time[1, 12]}] processing file $1..." >&2

sleep $((RANDOM % 4 + 1))

time=$(date +%T.%N)
echo "[${time[1, 12]}] file $1 processed" >&2

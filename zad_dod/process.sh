#!/bin/env zsh

time=$(date +%T.%N | cut -c 1-12)
echo "[$time] ($$) processing file $1..." >&2

sleep $((RANDOM % 4 + 1))

time=$(date +%T.%N | cut -c 1-12)
echo "[$time] ($$) file $1 processed" >&2

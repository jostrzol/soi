#!/bin/sh

N=4
name=$(basename "$0")
parent="$$"

if test $# -lt 2; then
    echo "usage:  $name PROCESS ARGUMENT [ARGUMENT...]" >&2
    exit 1
fi

command="$1"
shift

start_process() {
    $command "$1"
    until kill -USR1 $parent; do :; done
}

trap 'echo "ENTER HANDLE"; if test -n "$1"; then start_process "$1" & shift; fi; echo "END HANDLE"' USR1

for _ in $(seq $N); do
    if test -z "$1"; then
        break
    fi
    start_process "$1" &
    shift
done

until wait; do
    :
done

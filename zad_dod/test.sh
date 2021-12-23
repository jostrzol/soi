#!/bin/sh

N=8

sigusr1() {
    if test -n "$1"; then
        echo HANDLE "$1" >&2
        shift
        echo "$@"
        echo SETTING args TO "$*" >&2
    fi
}

trap 'if test -n "$1"; then echo HANDLE "$1" >&2; shift; fi' USR1

echo RUNNING "$$" with args "$@"

echo "CONSUME $1"
shift

echo "CONSUME $1"
shift

echo "CONSUME $1"
shift

while true; do
    sleep 1
done

wait

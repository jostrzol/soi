#!/bin/sh

N=4

name=$(basename "$0")
router="$$"

if test $# -lt 2; then
    echo "usage:  $name PROCESS ARGUMENT [ARGUMENT...]" >&2
    exit 1
fi

# prepare pipe and clean
pipe="/tmp/paralelize_pipe_$router"
set
clean() {
    # kill children
    # echo "CHILDREN:"
    # echo "========="
    # ps -o pid --ppid $$ | tail +2
    # echo "========="
    ps -o pid --ppid $$ | tail +2 | xargs kill 2>/dev/null
    ps -o pid --ppid $$ | tail +2 | xargs kill -9 2>/dev/null
    # echo "AFTER KILL:"
    # echo "========="
    # ps -o pid --ppid $$ | tail +2
    # echo "========="
    # remove pipe
    rm -f $pipe

    exit
}
trap 'clean' INT TERM
mkfifo "$pipe"

command="$1"
shift

start_process() {
    $command "$1"
    echo "done $1" >$pipe
}

to_run=$#
for _ in $(seq $N); do
    if test -z "$1"; then
        break
    fi
    start_process "$1" &
    shift
done

total_read=0
until test $total_read -eq $to_run; do
    read=$(cat $pipe | wc -l)
    total_read=$((total_read + read))
    for _ in $(seq $read); do
        if test -z "$1"; then break; fi
        start_process "$1" &
        shift
    done
done

rm $pipe

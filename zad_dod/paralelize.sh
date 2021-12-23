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
clean() {
    # kill children
    echo \$\$=$router
    echo "CHILDREN:"
    echo "========="
    ps -o pid,session --ppid $$
    echo "---------"
    ps -o pid -s $$
    echo "========="

    to_kill=$(ps -o pid= --ppid $$)
    kill $to_kill 2>/dev/null
    kill -9 $to_kill 2>/dev/null

    echo "AFTER KILL:"
    echo "========="
    ps -o pid= --ppid $$
    echo "---------"
    ps -o pid= -s $$
    echo "========="
    # remove pipe
    rm -f $pipe

    exit
}
trap 'clean' INT TERM
mkfifo "$pipe"

command="$1"
shift

start_process() {
    $command "$1" &
    trap 'echo "KILLING $command $1 ($!|$$)"; kill $!; kill -9 $!; exit' INT TERM
    wait
    echo "done $1" >$pipe
}

to_run=$#
for _ in $(seq $N); do
    if test -z "$1"; then
        break
    fi
    start_process "$1" &
    echo STARTING "$!"
    shift
done

clean

total_read=0
until test $total_read -eq $to_run; do
    read=$(cat $pipe | wc -l)
    total_read=$((total_read + read))
    for _ in $(seq "$read"); do
        if test -z "$1"; then break; fi
        start_process "$1" &
        shift
    done
done

rm $pipe

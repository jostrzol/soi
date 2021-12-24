#!/bin/sh

# global constants
N=4                            # max number of concurrent daemons to run
name=$(basename "$0")          # name of the script
pipe="/tmp/paralelize_pipe_$$" # interprocess comunication pipe
command="$1"                   # command to be run for daemons
shift                          #
to_run=$#                      # total number of daemons to run

# check args
if test $# -lt 2; then
    echo "usage:  $name PROCESS ARGUMENT [ARGUMENT...]" >&2
    exit 1
fi

# ensure clean exit
clean() {
    # kill wrappers for access to pipe
    to_kill=$(ps -o pid= --ppid $$)
    kill $to_kill 2>/dev/null
    kill -9 $to_kill 2>/dev/null

    # remove pipe
    rm $pipe

    exit
}
trap 'clean' INT TERM

# prepare pipe
mkfifo "$pipe"

# process pipe wrapper
start_process() {
    $command "$1"
    echo "done $1" >$pipe
}

# start initial N daemons
for _ in $(seq $N); do
    if test -z "$1"; then
        break
    fi
    start_process "$1" &
    shift
done

# handle finished daemons
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

clean

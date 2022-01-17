#!/bin/sh

cmd="../build/myfs"
fs_file="test.myfs"

$cmd create $fs_file

for i in $(seq 1024); do
    $cmd cp-to $fs_file small.txt "small${i}.txt"
done

if $cmd cp-to $fs_file small.txt small1025.txt 2>/dev/null; then
    echo >&2 "Full inodes, error expected!"
fi


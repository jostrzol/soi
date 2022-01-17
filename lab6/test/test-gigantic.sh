#!/bin/sh

cmd="../build/myfs"
fs_file="test.myfs"

$cmd create $fs_file

$cmd cp-to $fs_file gigantic gigantic
# 5 datablocks should be left

if $cmd cp-to $fs_file small.txt small.txt 2>/dev/null; then
    echo >&2 "Full datablocks, error expected!"
fi


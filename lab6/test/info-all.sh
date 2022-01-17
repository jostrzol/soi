#!/bin/sh

cmd="../build/myfs"
fs_file="test.myfs"

$cmd map $fs_file | uniq -c
echo "=============================="
$cmd info $fs_file
echo "=============================="
$cmd ls $fs_file
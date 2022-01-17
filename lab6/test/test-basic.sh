#!/bin/sh

cmd="../build/myfs"
fs_file="test.myfs"

$cmd create $fs_file
$cmd cp-to $fs_file small.txt small.txt
$cmd cp-from $fs_file small.txt /tmp/small.txt
diff -q /tmp/small.txt small.txt

$cmd cp-to $fs_file small.txt small2.txt
$cmd rm $fs_file small.txt
$cmd cp-to $fs_file big.txt big.txt
$cmd cp-from $fs_file small2.txt /tmp/small2.txt
$cmd cp-from $fs_file big.txt /tmp/big.txt
diff -q /tmp/small2.txt small.txt
diff -q /tmp/big.txt big.txt

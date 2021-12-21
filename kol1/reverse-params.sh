#!/bin/sh
reversed=""
while test "x$1" != "x"; do
	reversed="$1 $reversed"
	shift
done;
echo "$reversed"

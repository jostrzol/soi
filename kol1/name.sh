#!/bin/sh
progname=`basename $0`
case "$#" in
	"1") ;;
	"*") echo "usage: $progname UID" >&2; exit 1;;
esac
cat /etc/passwd | while IFS=: read name x UID rest; do
	if test "$UID" = "$1"; then
		echo $name
	fi
done 

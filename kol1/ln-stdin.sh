#!/bin/sh
ln=0
while read line; do
	echo $ln: $line
	ln=`expr $ln + 1`
done

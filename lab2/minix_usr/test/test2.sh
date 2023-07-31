#!/bin/sh
./proc_a &
./proc_b &
echo "Press F1 and see how many user process cycles were given to proc_a and proc_b"
echo "After the experiment kill the process manually (ps doesn't work)"

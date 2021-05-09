#!/bin/bash
for i in tests/*/
do
    printf "%s\n" $i
    ./TicketBooker "$i/input.txt">"$i/prog_output.txt"
    diff -b -E "$i/output.txt" "$i/prog_output.txt"
done
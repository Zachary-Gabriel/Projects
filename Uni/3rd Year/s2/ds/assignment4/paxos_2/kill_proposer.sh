#!/bin/bash

cd libpaxos/build

for i in {1..10}
do

    for j in {0..8}
    do
        ./sample/acceptor $j ../paxos.conf > dev/acceptor$j.txt &
    done

    ./sample/proposer 0 ../paxos.conf > dev/proposer0.txt &
    PID1=$!

    ./sample/proposer 1 ../paxos.conf > dev/proposer1.txt &
    PID2=$!

    ./sample/proposer 2 ../paxos.conf > dev/proposer2.txt &
    PID3=$!

    ./sample/learner ../paxos.conf > dev/learner.txt &
    ./sample/client ../paxos.conf > dev/client$i.txt &

    #execute order 66
    kill $PID1
    kill $PID2

    while (($SECONDS <= $i * 15))
    do
        var=0
    done

    kill $(jobs -p)

done
echo "done"
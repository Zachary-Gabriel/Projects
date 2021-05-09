#!/bin/bash

cd libpaxos/build

for i in {1..10}
do

    for j in {0..8}
    do
        ./sample/acceptor $j ../paxos.conf > dev/acceptor$j.txt &
        if (( $j == 0))
        then
            PID1=$!
        fi
        
        if (( $j == 2))
        then
            PID2=$!
        fi

        if (( $j == 5))
        then
            PID3=$!
        fi
    done

    for j in {0..2}
    do
        ./sample/proposer $j ../paxos.conf > dev/proposer$j.txt &
    done    

    ./sample/learner ../paxos.conf > dev/learner.txt &
    ./sample/client ../paxos.conf > dev/client$i.txt &

    #execute order 66
    kill $PID1
    kill $PID2
    kill $PID3

    while (($SECONDS <= $i * 15))
    do
        var=0
    done

    kill $(jobs -p)

done
echo "done"
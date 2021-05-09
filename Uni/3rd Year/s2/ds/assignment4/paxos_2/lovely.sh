#!/bin/bash

cd libpaxos/build

for i in {0..8}
do
    ./sample/acceptor $i ../paxos.conf > dev/acceptor$i.txt &
done

for i in {0..2}
do
    ./sample/proposer $i ../paxos.conf > dev/proposer$i.txt &
done

./sample/learner ../paxos.conf > dev/learner.txt &
./sample/client ../paxos.conf > dev/client0.txt &

while (($SECONDS <= 15))
do
    var=0
done

kill $(jobs -p)
echo "done"
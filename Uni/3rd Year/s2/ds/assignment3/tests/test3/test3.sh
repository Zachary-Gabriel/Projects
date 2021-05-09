#!/bin/bash

# The arguments for the executable objects are as follows:
# Runner {member ID} {largest member ID} {broadcast/ sniffing port} {total number of voters}
# Voter {member ID} {largest member ID} {broadcast/ sniffing port}

echo
echo "This is test 3."
echo "This test is designed to deal with 'n' amount of users."
echo "Unfortuneately, there will be no process crashing, as there seems to be issues regarding this."
echo 
echo "There are 10 prepare / proposition threads that are made at the same time."
echo "There are 20 voting threads."
echo "The idea is that the algorithm only needs to know the amount of total voters there are to reach a consensus."
echo
echo "It still takes about 15 seconds to run and kills the background jobs as its final step."
echo "The logs for all the processes will be logged in the 'output_logs' directory."
echo "At the bottom of each voter log file, it should tell you what the process voted."
echo "Ideally the resulting ID's are the same in every file."

for i in {1..10}
do
    java Runner $i 20 8080 19 > tests/test3/output_logs/Runner$i.log & 
done

# The rest of the bros start voting
for i in {1..20}
do
	java Voter $i 20 8080 > tests/test3/output_logs/Voter$i.log & 
done

# Kill the processes so that we dont run out of memory
while true
do
    if (($SECONDS >= 15))
    then
        # end test
        kill $(jobs -p)
        echo "Done."
        exit
    fi
done
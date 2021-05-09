#!/bin/bash

# The arguments for the executable objects are as follows:
# Runner {member ID} {largest member ID} {broadcast/ sniffing port} {total number of voters}
# Voter {member ID} {largest member ID} {broadcast/ sniffing port}

echo
echo "This is test 1."
echo "This test checks to make sure that 2 councillors can send voting proposals at the same time."
echo "This test initiates all 3 councillors at roughly the same time using multiple threads."
echo "It then slowly adds voters to the system every half a second until all 9 members are in the system."
echo
echo "It takes about 15 seconds to run and kills the background jobs as its final step."
echo "The logs for all the processes will be logged in the 'output_logs' directory."
echo "Notably, these logs tell you how the voting came to the conclusion that it did."
echo "At the bottom of each voter log file, it should tell you what the process voted."
echo "Ideally the resulting ID's are the same in every file."

# The three councilors will run their proposals at the same time (roughly)
for i in {1..3}
do
    java Runner $i 9 8080 9 > tests/test1/output_logs/Runner$i.log & 
done

# The rest of the bros start voting
for i in {1..9}
do
	java Voter $i 9 8080 > tests/test1/output_logs/Voter$i.log & 
    sleep 0.5
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
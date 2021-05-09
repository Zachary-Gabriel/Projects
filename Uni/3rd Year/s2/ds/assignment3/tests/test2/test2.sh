#!/bin/bash

# The arguments for the executable objects are as follows:
# Runner {member ID} {largest member ID} {broadcast/ sniffing port} {total number of voters}
# Voter {member ID} {largest member ID} {broadcast/ sniffing port}

echo
echo "This is test 2."
echo "The three councillors will ."
echo "This test starts by have the councillors making their propositions."
echo "It then calls all the voting threads all at roughly the same time."
echo "As the broadcast mechanism is multithreaded using Runnable, it should deal with these votes quickly."
echo
echo "It still takes about 15 seconds to run and kills the background jobs as its final step."
echo "The logs for all the processes will be logged in the 'output_logs' directory."
echo "At the bottom of each voter log file, it should tell you what the process voted."
echo "Ideally the resulting ID's are the same in every file."

# The three councillors will run their proposals at the same time (roughly)
for i in {1..3}
do
    java Runner $i 9 8080 9 > tests/test2/output_logs/Runner$i.log & 
done

wait 1

# The rest of the bros start voting at the same time
for i in {1..9}
do
	java Voter $i 9 8080 > tests/test2/output_logs/Voter$i.log & 
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
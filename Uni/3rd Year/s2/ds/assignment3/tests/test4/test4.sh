#!/bin/bash

# The arguments for the executable objects are as follows:
# Runner {member ID} {largest member ID} {broadcast/ sniffing port} {total number of voters}
# Voter {member ID} {largest member ID} {broadcast/ sniffing port}

echo
echo "This is test 4."
echo "This is to simulate the election of which this assignment is based on"
echo "Unfortuneately, there will be no process crashing, as there seems to be issues regarding this."
echo 
echo "There are 3 prepare / proposition threads. One will be loaded before voters start arriving,"
echo "another will arrive as the fourth member to arrive, and the last will be the 7th member to arrive."
echo "There are 9 voting threads in total."
echo "There is a 0.5 second gap between each member arriving to simulate time."
echo
echo "It still takes about 15 seconds to run and kills the background jobs as its final step."
echo "The logs for all the processes will be logged in the 'output_logs' directory."
echo "At the bottom of each voter log file, it should tell you what the process voted."
echo "Ideally the resulting ID's are the same in every file."

# The rest of the bros start voting
for i in {1..9}
do
    # Fast
    if (($i == 1))
    then
        java Runner 1 9 8080 9 > tests/test4/output_logs/Runner1.log & 
    fi

    # Average
    if (($i == 4))
    then
        java Runner 3 9 8080 9 > tests/test4/output_logs/Runner3.log & 
    fi

    # slow
    if (($i == 7))
    then
        java Runner 2 9 8080 9 > tests/test4/output_logs/Runner2.log & 
    fi

    # incrementally adding in the voters so that it appears that member 1 is fast, member 2 is slow and member 3 is average.
    java Voter $i 9 8080 > tests/test4/output_logs/Voter$i.log & 
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
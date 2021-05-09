#!/bin/bash

# description
echo "Running test 3!"
echo "This test checks the aggregate timer i.e. that no communication after 12 seconds removes the XML"
echo ""
echo "The first get client should get the aggregate's database when all 3 content servers have given the contents to the database."
echo "The second get client accesses the aggregate's database 12 seconds after it stops communicating with the second content server, meaning it should only have the contents of content server 1 and 3."
# setup
SECONDS=0
PID=0
$(cd ../../)

# aggregate server
java AggregationServer 8080 -v > tests/case3/logs/aggregate_server.log &

# content servers
for i in {1..3}
do
	java ContentServer localhost 8080 tests/case3/input$i.txt > tests/case3/logs/content_server$i.log &
    if (($i == 2))
    then
        PID=$!
    fi
done

# making sure the first GET client recieves all three xmls
while (($SECONDS < 3))
do 
    filler=$filler+1
done

java GETClient localhost 8080 -v > tests/case3/logs/GET_Client0.log &
BPID=$!
wait $BPID

# terminating the second content server's connection
kill $PID
SECONDS=0

# waiting 12 seconds
while (($SECONDS < 12))
do 
    filler=$filler+1
done

# checking if it worked
java GETClient localhost 8080 -v > tests/case3/logs/GET_Client1.log &
BPID=$!
wait $BPID

# end test
kill $(jobs -p)
echo "Done."
echo "Check the files in the 'logs' directory for more info."
exit

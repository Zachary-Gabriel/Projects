#!/bin/bash

# description
echo "Running test 1!"
echo "This test makes sure that the basic server interactions work."
echo ""
echo "The first client is processed before any of the content enters the aggregate server"
echo "The other 2 clients are processed at 10 and 20 seconds after the test starts."
echo "The content servers automatically start a heartbeat after they send the xml document to the aggregate server."
echo ""
echo "Essentially, this test makes sure the blank xml document is as intended. (Client 1 should have a blank xml)"
echo "It also makes sure the heartbeat works. (Clients 2 and 3 should be the same)"
echo "It also shows the lamport clock implementation. (Check the logs of the content and aggregate servers)"

# setup
SECONDS=0
$(cd ../../)

# aggregate server
java AggregationServer 8080 -v > tests/case1/logs/aggregate_server.log &

# first client
java GETClient localhost 8080 -v > tests/case1/logs/GET_Client0.log &
BPID=$!
wait $BPID

# content servers
for i in {1..3}
do
	java ContentServer localhost 8080 tests/case1/input$i.txt > tests/case1/logs/content_server$i.log &
done

while true
do
    if (($SECONDS == 10))
    then
        # second client
        java GETClient localhost 8080 -v > tests/case1/logs/GET_Client1.log &
        BPID=$!
        wait $BPID
    fi

    if (($SECONDS == 20))
    then
        # last client
        java GETClient localhost 8080 -v > tests/case1/logs/GET_Client2.log &
        BPID=$!
        wait $BPID

        # end test
        kill $(jobs -p)
        echo "Done."
        echo "Check the logs in the 'logs' directory for more info."
        exit
    fi
done
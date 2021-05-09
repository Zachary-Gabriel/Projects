#!/bin/bash

config_file="./config_fast.cfg"
acceptor_bin="../../acceptor"
learner_bin="../../client_learner"
client_bin="../../client_proposer_learner"

######################################################################
acceptor_offset=10;
learner_offset=10;
client_offset=200;

keep_win_open="echo -e \"\a\"; echo \"press enter to exit\"; read"

function bin_check () {
	for f in "$learner_bin" "$acceptor_bin" "$client_bin"; do
	if [[ ! -x $f ]]; then
		echo "File not found $f"
		exit 1;
	fi
done	
}


function launch_acceptor () {
	local acc_id=$1
	local launchcmd="$acceptor_bin $acc_id $config_file; $keep_win_open"
	local color="-bg black -fg red"
	local xtermoptions="$color -T acc$acc_id -geometry 80x30+"$acceptor_offset"+50"
	# echo "$launchcmd"
	xterm $xtermoptions -e "$launchcmd" &
	let 'acceptor_offset += 350'
	sleep 1;
}

function launch_learner () {
	local launchcmd="$learner_bin $config_file; $keep_win_open"
	local color="-bg black -fg blue"
	local xtermoptions="$color -T learner -geometry 90x30+$learner_offset+550"
	# echo "$launchcmd"
	xterm $xtermoptions -e "$launchcmd" &
	let 'learner_offset += 200'
	sleep 1;
}

function launch_client () {
	local launchcmd="$client_bin $config_file; $keep_win_open"
	local color="-bg black -fg green"
	local xtermoptions="$color -T client -geometry 80x30+$client_offset+550"
	# echo "$launchcmd"
	xterm $xtermoptions -e "$launchcmd" &
	let 'client_offset += 200'
	sleep 1;
}

######################################################################

bin_check

if [[ $(basename `pwd`) != "xterm_run_debug" ]]; then
	echo "$0 must be executed from the 'xterm_run_debug' directory"
	exit 1;
fi

launch_learner

launch_learner

acceptors=`cat $config_file | grep ^acceptor | sort -r | awk '{print $2}' | xargs`
for n in $acceptors; do
	echo "Starting acceptor $n"
	launch_acceptor $n
done

sleep 3
launch_client
wait

echo "All processes terminated"
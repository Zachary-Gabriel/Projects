Running the paxos algorithm:
Running 'make' will execute the test harness
Running 'make clean' will remove all the execess files made by the compiler.

There's a weird bug where if you run the tests 3 in a row with very little time between,
every process that tries to run will be halted due to the fact that the address has been taken already.

It works fine if you let it sit for 1 ~ 2 minutes between test runs.
Obviously this is less than ideal but it was only something I found after I brute forced my test suite.

Ideally this shouldn't be a problem, but the leftover processes seem to survive past the "kill jobs -p" command,
which is the extent of my understanding of jobs and processes in bash.

Terminology:
I used 'Runner' to describe the process of "running for president". As such, the object's name is runner.
I used 'Voter' to describe the process of voting.
I used a "number used once" or 'nonce' style of algorithm. Even though the number will be used more than once, after the number is discarded, that number won't be used again. 
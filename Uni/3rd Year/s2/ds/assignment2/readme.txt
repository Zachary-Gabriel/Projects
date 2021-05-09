If you want to run this program using the automated tests:
run "make"

--------------------------------------------------------------------------------------------
If you want to run this program manually, the instructions are as follows:

call "make setup" to compile the program
call "make aggregate" to start running the server

in another terminal, call "make content" to send some parsed input to the aggregate server.

in another terminal, call "make client" to get the XML documents from the aggregate server.

to remove clutter files when you're done, run "make clean".
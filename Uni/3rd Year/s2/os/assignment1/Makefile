build: TicketBooker
	./test.sh

TicketBooker: TicketBooker.cpp
	g++ -o TicketBooker TicketBooker.cpp 

# run using: make case CASE=8
case: TicketBooker
	./TicketBooker tests/case$$CASE/input.txt

clean:
	rm TicketBooker
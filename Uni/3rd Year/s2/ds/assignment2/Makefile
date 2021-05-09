run_tests: *.java
	javac *.java

	@echo "Run 'make clean' to tidy up the folder when you're finished";\
	echo "There are 2 tests in this submission. Enter an integer {1..2} to run the tests.";\
	read NUMBER;\
	./tests/case$$(NUMBER)/test.sh

startup: *.java
	javac *.java

client:
	java GETClient localhost 8080 -v

content:
	java ContentServer localhost 8080 tests/case1/input.txt

aggregate:
	java AggregationServer 8080

clean:
	rm *.class
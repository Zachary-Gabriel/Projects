# Runs the tests
run_tests: *.java
	javac *.java
	@echo "Run 'make clean' to tidy up the folder when you're finished."; \
	echo "There are 4 tests in this submission. Enter an integer {1,2,3,4} to run the tests."; \
	read number;\
	chmod 755 tests/test$$number/test$$number.sh; \
	./tests/test$$number/test$$number.sh

# Removes all the compiler made files and test logs
clean:
	rm *.class
	rm tests/test1/output_logs/*
	rm tests/test2/output_logs/*
	rm tests/test3/output_logs/*
	rm tests/test4/output_logs/*


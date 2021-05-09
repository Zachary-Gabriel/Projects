extern int * readNumbers(int n);
extern void printNumbers(int * numbers, int length);

int main(void) {
	int* array = new int[3];
	array = readNumbers(3);
	printNumbers(array,3);
	return 0;
}
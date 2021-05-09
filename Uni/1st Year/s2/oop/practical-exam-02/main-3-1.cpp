extern int* mergeArrays(int* A, int* B, int length);
extern int * readNumbers(int n);
extern void printNumbers(int * numbers, int length);

int main(void) {
	int * array = new int[6];
	int * A = new int[3];
	int * B = new int[3];

	A = readNumbers(3);
	B = readNumbers(3);
	array = mergeArrays(A,B,3);
	printNumbers(array,6);

	return 0;
}
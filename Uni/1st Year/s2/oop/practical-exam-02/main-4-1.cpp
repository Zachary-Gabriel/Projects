extern int * readNumbers(int n);
extern void printNumbers(int * numbers, int length);
void subArrays(int * array, int length);


int main(void) {
	int* array = new int[4];
	array = readNumbers(4);
	subArrays(array,4);
	delete[] array;
	return 0;
}
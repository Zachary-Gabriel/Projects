extern int * readNumbers(int n);
extern void grades (int *gradepoints, int length);

int main(void) {
	int* array = new int[3];
	array = readNumbers(3);
	grades(array,3);
	return 0;
}
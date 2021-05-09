extern int * readNumbers() ;
extern void printNumbers(int *numbers,int length) ;

int main(void) {
	int* array = readNumbers();
	printNumbers(array,10);
	return 0;
}
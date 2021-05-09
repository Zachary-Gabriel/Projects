extern int * readNumbers() ;
extern void hexDigits(int *numbers,int length) ;

int main(void) {
	int* array = readNumbers();
	hexDigits(array,10);
	return 0;
}
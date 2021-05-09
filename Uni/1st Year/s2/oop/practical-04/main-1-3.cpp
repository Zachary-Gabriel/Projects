
extern void cpyia(int*,int*,int) ;

int main(void) {
	
	int old[3]={1,2,3};
	int dupe[3];
	cpyia(old,dupe,3);

	return 0;
}
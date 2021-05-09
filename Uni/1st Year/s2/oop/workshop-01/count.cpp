// counts the number of even numbers in an array of integers with n elements
int count(int array[], int n)
{
    int numberOfEvens = 0;

    for(int i=0; i<n; i++) {
        if (array[i] %2 == 0)    // if a number is even then division by 2 will leave a remainder of 0
            numberOfEvens = numberOfEvens + 1;
    }

    return numberOfEvens;
}
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bn.h"

static int bn_resize(bn_t bn, int size) {
  if (size <= bn->bn_size)
    return 0;
uint16_t *data = (uint16_t *)realloc(bn->bn_data, size * sizeof(uint16_t));
if (data == NULL)
    return -1;
for (int i = bn->bn_size; i < size; i++)
    data[i] = 0;
bn->bn_data = data;
bn->bn_size = size;
return 1;
}

static int bn_reallen(bn_t bn) {
  int l = bn->bn_len;
  while (l-- > 0) {
    if (bn->bn_data[l] != 0)
      return l+1;
}
return 0;
}

static void dbn_push(bn_t bn, uint8_t data) {
  uint32_t carry = data;
  for (int j = 0; j < bn->bn_len; j++) {
    carry += bn->bn_data[j] * 256;
    bn->bn_data[j] = carry % 10000;
    carry = carry / 10000;
}
if (carry != 0)
    bn->bn_data[bn->bn_len++] = carry;
}

static bn_t todec(bn_t bn) {
  int binlen = bn_reallen(bn);
  int declen = ((binlen + 3)/4) * 5;
  bn_t dbn = bn_alloc();
  if (dbn == NULL)
    return NULL;
bn_resize(dbn, declen);
for (int i = binlen; i--; ) {
    dbn_push(dbn, bn->bn_data[i] >> 8);
    dbn_push(dbn, bn->bn_data[i] & 0xFF);
}
return dbn;
}

bn_t bn_alloc(void) {
  bn_t bn = (bn_t)malloc(sizeof(struct bn));
  if (bn == NULL)
    return NULL;
bn->bn_data = (uint16_t *)calloc(1, sizeof(uint16_t));
if (bn->bn_data == NULL) {
    free(bn);
    return NULL;
}
bn->bn_len = 0;
bn->bn_size = 1;
bn->bn_sign = 1;
return bn;
}

int bn_toString(bn_t bn, char *buf, int buflen) {
  bn_t dbn = todec(bn);
  if (dbn == NULL)
    return -1;
int dlen = dbn->bn_len;
uint16_t *data = dbn->bn_data;

int requiredlen;
if (dlen == 0)
    requiredlen = 2;
else
    requiredlen  = 2 + (bn->bn_sign < 0) + (dlen - 1) * 4 +
(data[dlen-1] > 999) + 
(data[dlen-1] > 99) + 
(data[dlen - 1] > 9);
if (requiredlen > buflen) {
    bn_free(dbn);
    return requiredlen;
}

char *p = buf;

if (dlen == 0) {
    *p++ = '0';
} else {
    if (bn->bn_sign < 0)
      *p++ = '-';
  dlen--;
  if (data[dlen] > 999)
      *p++ = '0' + (data[dlen] / 1000) % 10;
  if (data[dlen] > 99)
      *p++ = '0' + (data[dlen] / 100) % 10;
  if (data[dlen] > 9)
      *p++ = '0' + (data[dlen] / 10) % 10;
  *p++ = '0' + data[dlen] % 10;
  while (dlen--) {
      *p++ = '0' + (data[dlen] / 1000) % 10;
      *p++ = '0' + (data[dlen] / 100) % 10;
      *p++ = '0' + (data[dlen] / 10) % 10;
      *p++ = '0' + (data[dlen] / 1) % 10;
  }
}
*p = '\0';
bn_free(dbn);
return 0;
}

static uint16_t shiftleft(uint16_t *a, uint16_t *b, int n, int d) {
  uint32_t carry = 0;
  for (int i = 0; i < n; i++) {
    carry |= ((uint32_t)b[i]) << d;
    a[i] = carry & 0xffff;
    carry >>= 16;
}
return carry;
}


static uint32_t Step_D3(uint16_t *uj, uint16_t *v, int n) {
  uint32_t hat = (uj[n]<<16) + uj[n-1];
  uint32_t qhat = hat / v[n-1];
  uint32_t rhat = hat % v[n-1];
  if (qhat == 0x10000 || ( n>1 && (qhat * v[n-2] > 0x10000 * rhat + uj[n-2]))) {
    qhat--;
    rhat += v[n-1];
    if (rhat < 0x10000 && n>1 && (qhat * v[n-2] > 0x10000 * rhat + uj[n-2])) {
      qhat--;
      rhat += v[n-1];
  }
}
return qhat;
}

static uint16_t Step_D4(uint16_t *uj, uint16_t *v, uint32_t qhat, int n) {
  uint32_t borrow = 0;
  for (int i = 0; i < n; i++) {
    borrow += uj[i];
    borrow -= qhat * v[i];
    uj[i] = borrow & 0xFFFF;
    borrow >>= 16;
    if (borrow)
      borrow |= 0xFFFF0000; // The borrow is always non-positive...
}
borrow += uj[n];
uj[n] = borrow & 0xFFFF;
  return borrow >> 16; // The return value is 16 bits, so no need for extending the sign bit
}

static void Step_D6(uint16_t *uj, uint16_t *v, int n) {
  uint32_t carry = 0;
  for (int i = 0; i < n; i++) {
    carry += uj[i];
    carry += v[i];
    uj[i] = carry & 0xFFFF;
    carry >>= 16;
}
carry += uj[n];
uj[n] = carry & 0xFFFF;
  //assert(carry > 0xFFFF); // We ignore further carry
}

static void shiftright(uint16_t *u, int n, int d) {
  for (int i = 0; i < n; i++)
    u[i] = (u[i] >> d) | (u[i+1] << (16 - d));
u[n] >>= d;
}

// Using Algorithm 4.3.1 D of Knuth TAOCP
int bn_div(bn_t quotient, bn_t remainder, bn_t numerator, bn_t denominator) {
  // Use Knuth's variable names:
  //   u -- numerator
  //   v -- denominator
  //   q -- quotient
  //   d -- normalisation factor
  //   n -- length of denominator
  //   m -- length difference between numerator and denominator
  //   b -- base = 0x10000
  // Our base (b) is 2^16, so we can use the shift method to calculate d.
  // We use the space of the remainder for the normalised numerator, so
  // need to allocate another variable for that.
  if (numerator->bn_sign < 0 || denominator->bn_sign < 0)
    return -1;
if (quotient == numerator || 
  quotient == denominator || 
  quotient == remainder ||
  remainder == numerator ||
  remainder == denominator)
    return -1;

  // Step D1
int n = bn_reallen(denominator);
if (n == 0)
    return -1;
int d = 0;
uint16_t t = denominator->bn_data[n - 1];
  assert(t != 0); // This is OK from the calculation of n
  while ((t & 0x8000) == 0) {
    t <<= 1;
    d++;
}
bn_t vbn = bn_alloc();
bn_resize(vbn, n);
uint16_t *v = vbn->bn_data;
t = shiftleft(v, denominator->bn_data, n, d);
  // Not setting len of vbn because we do not really use it.
assert(t == 0);

int nl =  bn_reallen(numerator);
int m = nl < n ? 0 : nl - n;

remainder->bn_len = n;
bn_t ubn = remainder;
bn_resize(ubn, m + n + 1);
memset(ubn->bn_data, 0, (m + n + 1) * sizeof(uint16_t));
uint16_t *u = ubn->bn_data;
ubn->bn_data[nl] = shiftleft(u, numerator->bn_data, nl, d);

bn_resize(quotient, m + 1);
quotient->bn_len = m + 1;
uint16_t *q = quotient->bn_data;


  // Steps D2, D7
for (int j = m; j >= 0; j--) {
    // Step D3
    uint32_t qhat = Step_D3(u+j, v, n);

    // Step D4
    uint16_t borrow = Step_D4(u+j, v, qhat, n);
    
    // Step D5
    q[j] = qhat;
    if (borrow) {
      //Step D6
      assert(qhat != 0);
      Step_D6(u+j, v, n);
      q[j]--;
  }
}

  // Step D8
assert((u[0] & ((1<<d) - 1)) == 0);
shiftright(u, n, d);
bn_free(vbn);

return 0;
}



/************************************ HELPERS ****************************************/

// removes the header zeroes from a string
char* reduce (char* big_num)
{
    // finding length of string
    int len = 0;
    while (big_num[len] != '\0')
    {
        len++;
    }
    // slides the numbers forward
    while (big_num[0] == '0')
    {
        len--;
        for (int i = 0; i < len; ++i)
        {
            big_num[i] = big_num[i+1];
        }
        big_num[len] = '\0';
    }

    return big_num;
}

// multiplies 2 strings using base 10 (decimal)
char* dec_str_mult (char* a, char* b)
{
    // finding length of input strings
    int len_a = 0;
    while (a[len_a] != '\0')
    {
        len_a ++;
    }

    int len_b = 0;
    while (b[len_b] != '\0')
    {
        len_b ++;
    }

    // stores the number in an int array
    int* prod = calloc ((len_a + len_b + 1), sizeof (int));
    char* ret = calloc ((len_a + len_b + 2), sizeof (char));
    
    for (int i = len_a; i --; )
    {
        for (int j = len_b; j --; )
        {
            // multiplying each digit
            int res = (a [i] - 48) * (b [j] - 48);
            
            // adding to appropriate digit
            prod [i + j + 1] += res;

            // making sure each digit is between 0 & 9
            while (prod [i + j + 1] > 9)
            {
                prod [i + j + 1] -= 10;
                prod [i + j] ++;
            }
        }
    }

    // translating the int array to a char array
    for (int i = 0; i < len_a + len_b; ++i)
    {
        ret[i] = (char) (prod[i] + 48);
    }
    ret[len_a + len_b] = '\0';
    free (prod);
    ret = reduce (ret);
    return ret;
}

// returns true if bn > base
bool str_bn_lt (char* bn, int bn_len, char* base, int base_len)
{
    if (bn_len > base_len)
    {
        return true;
    }
    else if (bn_len < base_len)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < bn_len; ++i)
        {
            if (bn[i] > base[i])
            {
                return true;
            }
            else if (bn[i] < base[i])
            {
                return false;
            }
        }
    }
    return true;
}

// ret = a - b and increment appropriate index of bn
char* str_sub_dec (bn_t bn, char* a, char* b, int index )
{
    // getting length of a
    int a_len = 0;
    while (a[a_len] != '\0')
    {
        a_len ++ ;
    }

    // getting length of b
    int b_len = 0;
    while (b[b_len] != '\0')
    {
        b_len++ ;
    }

    // making a return char* and storing all valid *a digits
    char* ret = calloc (a_len+1, sizeof(char));
    int* working = calloc (a_len, sizeof(int));
    
    // as a >= b
    for (int i = 0; i < a_len; ++i)
    {
        working[i] = a[i]-48;
    }
    
    for (int i = a_len; i--;)
    {
        // if valid digit for b, do ret = a - b
        if (i >= (a_len - b_len))
        {
            working[i] -= b[i - (a_len - b_len)];
            working[i] += 48;

        }
        
        // dealing with carry
        if (working[i] < 0)
        {
            working[i] += 10;
            working[i-1] --;
        }
    }

    for (int i = 0; i < a_len; ++i)
    {
        ret[i] = working[i] + 48;
    }

    free (working);
    // storing the results to bn
    bn-> bn_data[index] ++;
    
    // removing the padded zeros from ret   
    ret = reduce (ret);
        
    return ret;
}

// shuffle down the input int arr to a big number
int shuffle_down (bn_t bn, char* dec, int length)
{
    // initilaising variables
    char* base = "65536\0"; // to make it easier to read
    int index = 0; // index of the bn_data that will be incremented

    char* power = "1\0"; // the value of the power associated to the index
    int power_len = 1; // the length of the char array above

    char* next_power = "65536\0"; // the value of the power that is too big to construct the number
    int next_pow_len = 5; // the length of the char array above
    
    // removing padding zeros
    dec = reduce (dec);
    // reducing the input char array until it is zero
    while (dec[0] != 0)
    {
        // initilaising variables
        index = 0;
        
        power = "1\0";
        power_len = 1;

        next_power = "65536\0";
        next_pow_len = 5;

        // finding the appropriate base
        while (str_bn_lt (dec, length, next_power, next_pow_len))
        {
            index ++;
            power = next_power;
            power_len = next_pow_len;

            next_power = dec_str_mult (next_power, base);
            next_pow_len = 0;
            while (next_power[next_pow_len] != '\0')
            {
                next_pow_len ++;
            }
        }
        
        // removing the base from the char array and storing the progress in the bn struct
        char* temp_dec = str_sub_dec (bn, dec, power, index);
        free (dec);
        dec = temp_dec;
        
        // resetting the variables for the next iteration
        length = 0;
        while (dec[length] != 0)
        {
            length ++;
        }
    }
    
    return 0;
}

// makes a duplicate bn
bn_t duplicate (bn_t a)
{
    bn_t new_bn = bn_alloc ();
    new_bn-> bn_len = a-> bn_len;
    new_bn-> bn_size = a-> bn_size;
    new_bn-> bn_sign = a-> bn_sign;
    new_bn-> bn_data = calloc (a-> bn_size, sizeof (uint16_t));
    for (int i = 0; i < a-> bn_len; ++i) 
    {
        new_bn-> bn_data[i] = a-> bn_data[i];
    }
    return new_bn;
}

// if bn_data of a > b, return true, otherwise false
bool bn_gt (bn_t a, bn_t b)
{
    int a_len = bn_reallen (a);
    int b_len = bn_reallen (b);

    if (a_len > b_len)
    {
        return true;
    }
    else if (a_len < b_len)
    {
        return false;
    }
    else
    {
        for (int i = a_len; i --; )
        {
            if (a->bn_data[i] < b->bn_data[i])
            {
                return false;
            }
            else if (a->bn_data[i] > b->bn_data[i])
            {
                return true;
            }
        }
        return false;
    }
}

// subtracting via output = a - b where a is larger
int sub_driver_bn (bn_t output, bn_t in_a, bn_t in_b)
{
    // in case output == a || b
    bn_t a = duplicate (in_a);
    bn_t b = duplicate (in_b);
    
    // error checking
    if (a == NULL) return 1;
    if (b == NULL) return 1;

    for (int i = 0; i < a->bn_len; ++i)
    {
        output -> bn_data[i] = a->bn_data[i];
    }
    output-> bn_sign = a-> bn_sign;
    output-> bn_len = a-> bn_len;
    int len = b-> bn_len;

    // looping through the digits
    for (int i = 0; i < len; ++i)
    {
        // if digit is negitive, take one from next digit
        if (output-> bn_data[i] < b-> bn_data[i])
        {
            output-> bn_data[i] -= b-> bn_data[i];
            output-> bn_data[i] += 65536; // this probably does nothing
            
            if (i < (output-> bn_len -1))
            {
                output-> bn_data[i+1] --;
            }
        }
        else
        {
            output-> bn_data[i] -= b-> bn_data[i];
        }
    }

    // if zero, set sign to +ve
    for (int i = 0; i < len; ++i)
    {
        if (output->bn_data[i] == 0)
        {
            if (i == len-1)
            {
                output-> bn_sign == 1;
            }
            continue;
        }
        else
        {
            break;
        }
    }

    // free mem
    bn_free (a);
    bn_free (b);
    return 0;
}

bool not_zero (bn_t in)
{
    in-> bn_len = in-> bn_size;
    in-> bn_len = bn_reallen (in);
    
    for (int i = 0; i < in->bn_len; ++i)
    {
        if (in-> bn_data[i] != 0)
        {
            return true;
        }
    }
    return false;
}

void fail_check (int in)
{
	if (in != 0)
    {
        exit (in);
    }
}

// fixes the issues with bignum
void true_bn_reallen (bn_t bignum)
{
    bignum-> bn_len = bignum-> bn_size;
    bignum-> bn_len = bn_reallen (bignum);
}

int bn_small (bn_t bignum, int val)
{
    bignum-> bn_sign = 1;
    bignum-> bn_len = 1;
    bignum-> bn_size = 1;
    bignum-> bn_data[0] = val;
    return 0;
} 

// modulo arithamtic 
int bn_mod (bn_t result, bn_t base, bn_t modulus)
{
    bn_t quotient = bn_alloc();
	int ret = bn_div (quotient, result, duplicate(base), duplicate(modulus));
    bn_free (quotient);
	fail_check (ret);
    return 0;
}

// bn_div (bn_t quotient, bn_t remainder, bn_t numerator, bn_t denominator)

// 
void bn_exp (bn_t result, bn_t base, bn_t exp, bn_t bn_two)
{
    bn_t rem = bn_alloc ();
    bn_t dupe_exp = duplicate (exp);

    bn_div (exp, rem, dupe_exp, bn_two);
    
    true_bn_reallen (exp);
    true_bn_reallen (rem);
    true_bn_reallen (dupe_exp);
    true_bn_reallen (bn_two);

    if (not_zero (rem))
    {
        bn_mul (result, result, base);
    }

    bn_free (rem);
    bn_free (dupe_exp);
}

// bignum number power
int bn_exp_driver (bn_t result, bn_t base, bn_t exp, bn_t mod)
{
    bn_t bn_two = bn_alloc ();
    bn_small (bn_two, 2);

    while (not_zero (exp))
    {
        bn_exp (result, base, exp, bn_two);
        bn_mul (base, base, base);
        bn_mod (result, result, mod);
        bn_mod (base, base, mod);
    }

    bn_free (bn_two);
    return 0;
}

/************************************* MAIN ***************************************/

// res = a + b
int bn_add (bn_t output, bn_t a, bn_t b) 
{
    a-> bn_len = bn_reallen(a);
    b-> bn_len = bn_reallen(b);

    // in case output == a || b
    bn_t dupe_a = duplicate (a);
    bn_t dupe_b = duplicate (b);

    // gets the larger number
    int len = a-> bn_len;
    if (len < b-> bn_len)
    {
        len = b-> bn_len;
    }
    
    // changing output
    free (output-> bn_data);
    output-> bn_data = calloc (len+2, sizeof(uint16_t));
    output-> bn_len = len+2;
    output-> bn_size = len+2;
    output-> bn_sign = 1;
    if (output == NULL) return 1;

    // adding
    if ((dupe_a-> bn_sign) * (dupe_b-> bn_sign) == 1)
    {
        output-> bn_sign = dupe_a-> bn_sign;
        for (int i = 0; i < len; ++i)
        {
            // adding a to the result
            if (i < dupe_a-> bn_len)
            {
                output-> bn_data[i] += (dupe_a-> bn_data[i]);
            }
            // adding b to the result
            if (i < dupe_b-> bn_len)
            {
                output-> bn_data[i] += (dupe_b-> bn_data[i]);
            }

            // dealing with carries using overflow
            if (dupe_a-> bn_data[i] > output-> bn_data[i])
            {
                output-> bn_data[i] -= 65536;
                output-> bn_data[i+1] ++;
            }
        }
        output-> bn_len = bn_reallen (output);
        bn_free (dupe_a);
        bn_free (dupe_b);
        return 0; 
    }

    else if (bn_gt (dupe_a, dupe_b))
    {
        int ret = sub_driver_bn (output, dupe_a, dupe_b);
        bn_free (dupe_a);
        bn_free (dupe_b);
        return ret;
    }

    else
    {
        int ret = sub_driver_bn (output, dupe_b, dupe_a);
        bn_free (dupe_a);
        bn_free (dupe_b);
        return ret;
    }
}

// res = a - b
int bn_sub (bn_t result, bn_t a, bn_t b) 
{
    bn_t dupe_b = duplicate (b);
    dupe_b-> bn_sign *= -1;
    return bn_add (result, a, dupe_b);
}

// res = a * b
int bn_mul (bn_t result, bn_t in_a, bn_t in_b) 
{
    bn_t a = duplicate (in_a);
    bn_t b = duplicate (in_b);
    
    // finding length of input strings
    int len_a = bn_reallen (a);
    int len_b = bn_reallen (b);

    // changing result
    free (result-> bn_data);
    result-> bn_data = calloc ((len_a + len_b), sizeof (uint16_t));
    result-> bn_len = (len_a + len_b);
    result-> bn_size = (len_a + len_b);
    result-> bn_sign = a-> bn_sign * b->bn_sign;

    // stores the number in an int array
    int* prod = calloc ((len_a+len_b), sizeof (uint32_t));
    
    for (int i = 0; i < len_a; i ++)
    {
        for (int j = 0; j < len_b; j++)
        {
            // multiplying each digit
            uint32_t res = (a-> bn_data[i]) * (b-> bn_data[j]);
            
            // adding to appropriate digit
            prod[i+j] += res;
            
            // making sure each digit is between 0 & 65536
            while ((uint32_t) prod[i+j] >= 65536)
            {
                prod[i+j] -= 65536;
                prod[i+j+1] ++;
            }
        }
    }

    for (int i = 0; i < result->bn_len; ++i)
    {
        result-> bn_data[i] = (uint16_t) prod[i];
    }
    return 0; 
}

// converts string to bn
int bn_fromString (bn_t bn, const char *s) 
{
    // for the sake of finding the first digit in *s
    int index = 0;

    // checking the sign
    bn-> bn_sign = 1;
    if (s[0] == '-')
    {
        bn-> bn_sign = -1;
        ++ index;
    }

    // finding length
    int length = 0;
    while (s[index + length] != '\0')
    {
        if (48 <= (int) s[index] && (int) s[index] <= 57)
        {
            // do nothing
        }
        else
        {
            // spit the dummy
            return -1;
        }
        length++;
    }

    // allocating memory for the new char array that is only digits
    char* dec = calloc (length + 1, sizeof(char));
    for (int i = 0; i < length; ++i)
    {
        dec[i] = s[index];
        index ++;
    }

    // getting an estimate for how many digits will be needed to store the bn
    char* next_power = "65536\0";
    int next_pow_len = 5;

    index = 0;
    while (str_bn_lt (dec, length, next_power, next_pow_len))
    {
        index ++;
        
        char* temp = dec_str_mult (next_power, "65536\0");
        if (index != 1) free (next_power);
        next_power = temp;

        next_pow_len = 0;
        while (next_power[next_pow_len] != '\0')
        {
            next_pow_len ++;
        }
    }
    // allocating memory for bn_data, and changing appropriate elements about the struct
    free (bn-> bn_data);
    bn-> bn_data = calloc (index + 1, sizeof(uint16_t));
    bn-> bn_len = index + 1;
    bn-> bn_size = index + 1;
    // reduces the char array dec, and stores the result in bn
    return shuffle_down (bn, dec, length);
}

// frees the bn memory
void bn_free(bn_t bn) 
{
    if (bn == NULL)
        return ;
    
    bn-> bn_len = 0;
    bn-> bn_size = 0;
    bn-> bn_sign = 0;

    if (bn-> bn_data != NULL) 
    {
        free (bn-> bn_data);
    }
    free (bn);
    return ;
}

// assignment 5 bn mod and exponent as
int bn_modexp (bn_t result, bn_t base, bn_t exp, bn_t modulus) 
{ 
    // allocating memory
    bn_t local_base = duplicate (base);
    bn_t local_exp = duplicate (exp);
    bn_t local_mod = duplicate (modulus);

	// resetting result to 1 (the multiplicative nulifier)
    int ret = bn_resize (result, 1);
    fail_check (ret);
    bn_small (result, 1);

    // making the large number
    bn_exp_driver (result, local_base, local_exp, modulus);

	// shifting so 0 <= result < modulus
    ret = bn_mod (result, result, local_mod); 
	fail_check (ret);

    bn_free (local_base);
    bn_free (local_mod);
    bn_free (local_exp);
    return ret;
}
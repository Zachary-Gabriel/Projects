	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	# Test positive value, zero shift
	li		t0, 0x123456785A6B7C8D
	li		t2, 0
	sraw		t1, t0, t2
	TEST_EXPECT	t1, 0x000000005A6B7C8D

	# Test positive value, non-zero shift
	li		t0, 0x123456785A6B7C8D
	li		t2, 8
	sraw		t1, t0, t2
	TEST_EXPECT	t1, 0x00000000005A6B7C

	# Test positive value, non-zero shift
	li		t0, 0x123456785A6B7C8D
	li		t2, 48
	sraw		t1, t0, t2
	TEST_EXPECT	t1, 0x0000000000005A6B

	# Test negative value, zero shift
	li		t0, 0x12345678A5B6C7D8
	li		t2, 0
	sraw		t1, t0, t2
	TEST_EXPECT	t1, 0xFFFFFFFFA5B6C7D8

	# Test negative value, non-zero shift
	li		t0, 0x12345678A5B6C7D8
	li		t2, 20
	sraw		t1, t0, t2
	TEST_EXPECT	t1, 0xFFFFFFFFFFFFFA5B

	# Test negative value, non-zero shift
	li		t0, 0x12345678A5B6C7D8
	li		t2, 44
	sraw		t1, t0, t2
	TEST_EXPECT	t1, 0xFFFFFFFFFFFA5B6C

	# ################################################################

	# Test with x0 as destination
	li		t0, 0x12345678A5B6C7D8
	li		t2, 20
	sraw		zero, t0, t2
	TEST_EXPECT	zero, 0x0000000000000000
	

	TEST_END

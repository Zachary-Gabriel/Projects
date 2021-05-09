#include <stdint.h>
#include <stdio.h>
#define O(type, field) (size_t)(&(((type *)0)->field))
struct byname {
	int16_t int16;
	int32_t int32;
	int64_t int64;
	int8_t int8;
	intptr_t intptr;
	uint16_t uint16;
	uint32_t uint32;
	uint64_t uint64;
	uint8_t uint8;
	uintptr_t uintptr;
};

struct bysize {
	int8_t int8; // 1 byte
	uint8_t uint8;
	int16_t int16; // 2 byte
	uint16_t uint16;
	int32_t int32; // 4 byte
	uint32_t uint32;
	int64_t int64; // 8 byte
	uint64_t uint64;
	intptr_t intptr;
	uintptr_t uintptr;
};

struct min {
	uint64_t uint64;
	int64_t int64;
	intptr_t intptr;  
	uintptr_t uintptr;
	int32_t int32; 
	uint32_t uint32;  
	int16_t int16; 
	uint16_t uint16; 
	int8_t int8; 
	uint8_t uint8; 
};

struct max {
	uint64_t uint64;
	uint8_t uint8; 
	int64_t int64;
	int8_t int8; 
	intptr_t intptr;  
	uint16_t uint16; 
	uintptr_t uintptr;
	int16_t int16; 
	int32_t int32; 
	uint32_t uint32;  
};

int main() {
	printf("By name: %u\n", sizeof(struct byname));
	printf("By size: %u\n", sizeof(struct bysize));
	printf("By min: %u\n", sizeof(struct min));
	printf("By max: %u\n", sizeof(struct max));
}

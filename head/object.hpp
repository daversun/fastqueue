#include <iostream>
#include <stdint.h>

#ifndef OBJECT
#define OBJECT
class Object{
	public:
		uint32_t 	len;								// the length of obj
		uint8_t* 	addr;  								// the address of obj
		Object(uint8_t* addr, int len):len(len),addr(addr){}
		
		
};

#endif
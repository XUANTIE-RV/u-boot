#include "../include/common_lib.h"


void wr(u64 addr, u32 data) 
{
	*(volatile unsigned int*)(addr) = data;
}

u32 rd(u64 addr)
{
	return *(volatile unsigned int*)(addr);
}

void wr8(u64 addr, u8 data)
{
	*(volatile unsigned char*)(addr) = data;
}

u8 rd8(u64 addr)
{
	return *(volatile unsigned char*)(addr);
}

void wr16(u64 addr, u16 data)
{
	*(volatile unsigned short*)(addr) = data;
}

u16 rd16(u64 addr)
{
	return *(volatile unsigned short*)(addr);
}

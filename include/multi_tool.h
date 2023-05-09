#ifndef __MULTI_TOOL_H_
#define __MULTI_TOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "base64.h"

///////////////////////
// Mudule Index


typedef struct TIME_NOW
{
	short int tm_year;
	short int tm_mon;
	short int tm_day;
	short int tm_hour;
	short int tm_min;
	short int tm_sec;

}TIME_NOW;

typedef struct MEMORY_NOW
{
	long int mem_unit;
	long int totalram;
	long int freeram;
	long int bufferram;
	long int totalswap;
	long int freeswap;
	
} MEMORY_NOW;


//pthread_t TimeWatch_t;
MEMORY_NOW GET_MEMORY_NOW(MEMORY_NOW *set_info);
TIME_NOW GET_TIME_NOW(TIME_NOW *set_time);

int select_range_random_num_generate(int range, int seet);
void Memset(void *dt, size_t zero_size);
char* cMalloc(size_t bsize);

void Format(char *buf, size_t size ,char *fm, const char* format, ...);
void mFormat_Encoding(char *buf, size_t size, char *fm, const char *format, ...);


/*
void Print(char *fm, const char *format, ...);
void PrintDebugs(char *fm, const char *format, ...);
*/

int char_int(char* str_);


#endif

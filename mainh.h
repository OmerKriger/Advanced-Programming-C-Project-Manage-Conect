#pragma once
// defines
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "trees.h"
// structs
typedef struct
{
	unsigned short insId;
	float price;
} MusicianPriceInstrument;

typedef struct
{
	char** name;
	//MPIList instruments;
}Musician;

typedef struct
{
	int day, month, year;
	float hour;
} Date;

typedef struct
{
	int num;
	int inst;
	char importance;
}ConcertInstrument;

typedef struct
{
	Date date_of_concert;
	char* name;
	//CIList instruments;
} Concert;

void* checkAllocation(void* ptr);

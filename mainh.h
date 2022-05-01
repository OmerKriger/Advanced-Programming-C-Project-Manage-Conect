#pragma once
// defines
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "files.h"
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
	MusicianPriceInstrument* instruments;
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

// Functions Declerations
void* checkAllocation(void* ptr);
Musician** BuildMusiciansGroup(char* fileName, InstrumentTree InstTree);

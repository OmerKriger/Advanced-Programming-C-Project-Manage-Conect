// Includes
#include "mainh.h"
#include "trees.h"
#include "lists.h"
#include "files.h"
// functions

void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* MusicianCollection);
char* getLineOfText();
Concert getConcertData(char* data, InstrumentTree InstTree);
Date getDate();
void reserveMusicians(Concert concert);

void main(int argc, char* argv[])
{	
	unsigned short instAmount;

	if (!checkFilePaths(argc, argv, FILES))
	{
		printf("Missing name/paths of files");
		return;
	}

	InstrumentTree InstTree = BuildInstTree(argv[INSTRUMENTS], &instAmount);
	Musician** MusiciansGroup = BuildMusiciansGroup(argv[MUSICIANS], InstTree);
	MusiciansCollection* MusicianCollection = BuildMusiciansCollection(MusiciansGroup, instAmount);
	getShow(InstTree, MusiciansGroup, MusicianCollection);
	
}

void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* MusicianCollection)
{
	char* str;
	Concert concert;
	str = getLineOfText();
	while (str != NULL)
	{
		concert = getConcertData(str,InstTree);
		reserveMusicians(concert);
		str = getLineOfText();
	} 

}
Concert getConcertData(char* data, InstrumentTree InstTree)
{
	Concert concert;
	char* token, seps[] = SEPS2;
	token = strtok(data, seps);
	concert.name = _strdup(token); // copy the name of conert
	concert.date_of_concert = getDate();
	concert.instruments = createCIList(InstTree);
	return concert;
}
char* getLineOfText()
{
	short pSize = 1, lSize = 0;
	char* str = (char*)malloc(sizeof(char) * pSize);
	str = checkAllocation(str);
	str[0] = '\0';
	char c = getchar();
	while (c != '\n')
	{
		if (lSize + 1 == pSize)
		{
			pSize *= 2;
			char *tmp = (char*)realloc(str, sizeof(char) * pSize);
			str = checkAllocation(tmp);
		}
		str[lSize] = c;
		lSize++;
		str[lSize] = '\0';
		c = getchar();
	}
	pSize = lSize + 1;
	char *tmp = (char*)realloc(str, sizeof(char) * pSize);
	str = checkAllocation(str);
	return str;
}
Date getDate()
{
	short hour, min;
	char* token, seps[] = SEPS2;
	Date date;
	token = strtok(NULL, seps);
	date.day = atoi(token);
	token = strtok(NULL, seps);
	date.month = atoi(token);
	token = strtok(NULL, seps);
	date.year = atoi(token);
	token = strtok(NULL, seps);
	hour = atoi(token);
	token = strtok(NULL, seps);
	min = atoi(token);
	date.hour = (float)hour + (float)(min / 60);
	return date;
}
void reserveMusicians(Concert concert)
{
}

void* checkAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Error!");
		exit(1);
	}
	else return ptr;
}
void checkSTRtok(char* ptr)
{
	if (ptr == NULL)
	{
		printf("error in creating musician");
		exit(1);
	}
}





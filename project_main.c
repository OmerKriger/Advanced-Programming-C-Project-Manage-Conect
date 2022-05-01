// Includes
#include "mainh.h"
// Defines
#define FILES 2
#define INSTRUMENTS 1
#define MUSICIANS 2
#define SEPS " ,.;:?!-\t'()[]{}<>~_"
// functions declerations

Musician* CreateMusician(char* line);

// functions
void main(int argc, char** argv)
{	
	if (!checkFilePaths(argc, argv, FILES))
	{
		printf("Missing name/paths of files");
		return;
	}
	InstrumentTree InstTree = BuildInstTree(argv[INSTRUMENTS]);
	Musician** MusiciansGroup = BuildMusiciansGroup(argv[MUSICIANS], InstTree);
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
// NOT SURE WHERE PUT THIS Functions

Musician** BuildMusiciansGroup(char* fileName, InstrumentTree InstTree)
{
	FILE* f = fopen(fileName,'r');
	int pSize=1,lSize=0;
	Musician** musicians = (Musician*)malloc(sizeof(Musician*) * (pSize));
	musicians = checkAllocation(musicians);
	musicians[0] = NULL;
	char* line = getLineFromFile(f);
	while (line != EOF)
	{
		if (pSize == lSize + 1)
		{
			pSize *= 2;
			Musician** tmp = (Musician*)realloc(musicians,sizeof(Musician*) * (pSize));
			musicians = checkAllocation(tmp);
		}
		musicians[lSize]= CreateMusician(line,InstTree);
		lSize++;
		musicians[lSize] = NULL;
	}
	pSize = lSize + 1;
	Musician** tmp = (Musician*)realloc(musicians, sizeof(Musician*) * (pSize));
	musicians = checkAllocation(tmp);
	return musicians;
}

Musician* CreateMusician(char* line, InstrumentTree InstTree) // NEED to make it look better and shorter
{
	Musician* musician = (Musician*) malloc(sizeof(Musician));
	musician = checkAllocation(musician);
	char* name = (char*)malloc(sizeof(char)*strlen(line));
	name = checkAllocation(name);
	char seps[] = SEPS,*token = NULL;

	// ~~~~~~ from here should get to new function ~~~~~~~~
	// full name must include at least 2 names
	token = strtok(line,seps); // grapping the first
	checkSTRtok(token);
	strcpy(name, token); // copy to the name
	token = strtok(line, seps); // grapping the first
	checkSTRtok(token);
	strcat(name, " "); // add space
	strcat(name, token); // add the second name
	// ~~~~~~~~~~~~~~~~~~ till here ~~~~~~~~~~~~~~~~~~~~~~~~
	// grapping the rest 
	int instrumentID;
	token = strtok(line, seps);
	checkSTRtok(token);
	instrumentID = findInsId(InstTree, token);
	while (instrumentID == NOT_FOUND)
	{
		strcat(name, " "); // add space
		strcat(name, token); // add the name to the full name
		token = strtok(line, seps);
		checkSTRtok(token);
	}
	// NEED to Continue from here (instruments) - the first instrument in TOKEN pointer

}
// Includes
#include "mainh.h"
// Defines
#define FILES 2
#define INSTRUMENTS 1
#define MUSICIANS 2
#define SEPS " ,.;:?!-\t'()[]{}<>~_"
// functions declerations

Musician* CreateMusician(char* line, InstrumentTree InstTree);
char** getMusicianName(char** pToken, char* line, InstrumentTree InstTree);
Musician** BuildMusiciansGroup(char fileName[], InstrumentTree InstTree);
void makeEmptyList(MPIList* list);
bool isEmptyList(MPIList* list);
MPI* CreateMPInode(int ID, float price);
void CreateMPIList(char* token,char* line, InstrumentTree InstTree, MPIList* list);
void appendNodeToList(MPIList* list, MPI* node);
Musician*** BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount);



// functions
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
	Musician*** MusicianCollection = BuildMusiciansCollection(MusiciansGroup, &instAmount);
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

Musician** BuildMusiciansGroup(char fileName[], InstrumentTree InstTree)
{
	FILE* f = fopen(fileName,"r"); // open the file of the musicians
	checkOpenFile(f);
	int pSize=1,lSize=0; // create default sizes for array
	Musician** musicians = (Musician**)malloc(sizeof(Musician*) * (pSize));
	musicians = checkAllocation(musicians);
	musicians[0] = NULL;
	char* line = getLineFromFile(f);
	while (line != NULL) // run till the file is ending
	{
		if (pSize == lSize + 1) // check if there are enough space in the array.
		{
			pSize *= 2;
			Musician** tmp = (Musician**)realloc(musicians,sizeof(Musician*) * (pSize));
			musicians = checkAllocation(tmp);
		}
		musicians[lSize]= CreateMusician(line,InstTree); // create musician data and put it in the array
		lSize++;
		musicians[lSize] = NULL; // put the mark of last musician in array
		line = getLineFromFile(f); // get new line for next round
	}
	pSize = lSize + 1;
	Musician** tmp = (Musician**)realloc(musicians, sizeof(Musician*) * (pSize)); // make the array tite (last cell is NULL for marking end)
	musicians = checkAllocation(tmp); 
	return musicians; // return the array of pointers to musician
}

Musician* CreateMusician(char* line, InstrumentTree InstTree) 
{
	char seps[] = SEPS, * token = NULL;
	Musician* musician = (Musician*) malloc(sizeof(Musician));
	musician = checkAllocation(musician);
	musician->name = getMusicianName(&token,line,InstTree); // get the name from the string of line and return token the first instrument
	makeEmptyList(&(musician->instruments));
	CreateMPIList(token,line, InstTree,&(musician->instruments));
	return musician;
}

char** getMusicianName(char** pToken, char* line, InstrumentTree InstTree) // maybe need get it shorter ??????
{
	char seps[] = SEPS, * token = NULL;
	int instrumentID= NOT_FOUND ,pSize=2,lSize=0;
	char** fullName = (char**)malloc(sizeof(char*)*pSize);
	fullName = checkAllocation(fullName);
	token = strtok(line, seps); // grapping the first
	checkSTRtok(token); // check grapping 
	while (lSize <= 1 || instrumentID == NOT_FOUND)
	{
		if (pSize == lSize + 1)
		{
			pSize *= 2;
			char** tmp = realloc(fullName, sizeof(char*) * pSize);
			fullName = checkAllocation(tmp);
		}
		char* name = _strdup(token); 
		name = checkAllocation(name);
		fullName[lSize] = name;
		lSize++;
		fullName[lSize] = NULL;
		token = strtok(NULL,seps); // grapping next name/instrument in line
		checkSTRtok(token);
		if(lSize > 1)
			instrumentID = findInsId(InstTree, token); // check if the next grapping is instrument
	}
	pSize = lSize + 1;
	char** tmp = realloc(fullName, sizeof(char*) * pSize);
	fullName = checkAllocation(tmp);
	*pToken = token;
	return fullName;
}

void CreateMPIList(char* token, char* line, InstrumentTree InstTree, MPIList* list)
{
	char seps[] = SEPS;
	int instID,tmp;
	float price;
	while (token != NULL)
	{
		instID = findInsId(InstTree, token);
		token = strtok(NULL, seps);
		checkSTRtok(token);
		if (sscanf(token, "%d", &tmp) != 1)
		{
			printf("ERROR!, in creating MPIList");
			exit(1);
		}
		price = (float)tmp;
		MPI* node = CreateMPInode(instID, price);
		appendNodeToList(list, node);

		token = strtok(NULL, seps); // get the next instrument
	}
	free(line); // we finished with the this musician and can free the data line
}

MPI* CreateMPInode(int ID, float price)
{
	MPI* newNode = (MPI*)malloc(sizeof(MPI));
	newNode = checkAllocation(newNode);
	newNode->insId = ID;
	newNode->next = newNode->previous = NULL;
	newNode->price = price;
	return newNode;
}

void appendNodeToList(MPIList* list, MPI* node) // weird warning need to look
{
	if (isEmptyList(list))
	{
		list->head = list->tail = node;
	}
	else
	{
		list->tail->next = node;
		node->previous = list->tail;
		list->tail = node;
	}
}

void makeEmptyList(MPIList* list)
{
	list->head = list->tail = NULL;
}

bool isEmptyList(MPIList* list)
{
	if (list->head == NULL || list->tail == NULL)
		return true;
	else
		return false;
}



Musician*** BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount)
{
	Musician*** musicianCollection = (Musician***)malloc(sizeof(Musician**) * instAmount);
	musicianCollection = checkAllocation(musicianCollection);






}
// Includes
#include "mainh.h"
#include "trees.h"
#include "lists.h"
#include "files.h"

// functions
void sortMusicianCollection(MusiciansCollection* Mcollection, int instAmount);
void bubbleSort(Musician** arr, int size);
void swap(Musician** m1, Musician** m2);
void initializeIsChosen(Musician** musicians);
void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* MusicianCollection);
char* getLineOfText();
Concert getConcertData(char* data, InstrumentTree InstTree);
Date getDate();
void reserveMusicians(Concert concert);
void printMusiciansForConcert(MusiciansCollection* Mcollection, Concert concert);
void printConcertsDetails(Concert concert);
char* musicanName(char** arr, int* size);


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
	sortMusicianCollection(MusicianCollection, instAmount);
	initializeIsChosen(MusiciansGroup);
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

void sortMusicianCollection(MusiciansCollection* Mcollection, int instAmount)
{
	int i;
	for (i = 0; i < instAmount; i++)
		bubbleSort(Mcollection[i].pMusicians, Mcollection[i].logSize);
}

void printMusiciansForConcert(MusiciansCollection* Mcollection, Concert concert)
{
	char importance;
	int i, size, len , totalPrice = 0;
	char* str = NULL, * tmp;
	CINode* curr = concert.instruments.head;
	while (curr != NULL)
	{
		if (curr->importance == 0)
		{
			for (i = 0; i < Mcollection[curr->inst].logSize; i++)
			{

				if (Mcollection[curr->inst].pMusicians[i]->isChosen == false)
				{
					str = musicanName(Mcollection[curr->inst].pMusicians[i]->name, &size);
					len = strlen(curr->insName);
					len += 8;
					tmp = (char*)realloc(str, size + sizeof(char) * (len)); // 8 for spaces and commas/dots.
					str = checkAllocation(str);

					MPI* currInst = Mcollection[curr->inst].pMusicians[i]->instruments.head;
					while (currInst != NULL)
					{
						if (currInst->insId == curr->inst)
						{
							if (curr->next != NULL)
								sprintf(str, " - %s (%d), ", curr->insName, (int)currInst->price);
							else
								sprintf(str, " - %s (%d). ", curr->insName, (int)currInst->price);
							totalPrice += currInst->price;
						}
						currInst = currInst->next;
					}
				}
			}
		}
		curr = curr->next;
	}
	if (str == NULL)
		printf("Could not find musicians for the concert %s", concert.name);
	else
	{
		printf("%s", str);
		printf("Toatal cost: %d", totalPrice);
	}
}


void printConcertsDetails(Concert concert)
{
	printf("%s", concert.name);
	if (concert.date_of_concert.day < 10 && concert.date_of_concert.month >= 10)
		printf(" 0%d %d %d ", concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	else if (concert.date_of_concert.day < 10 && concert.date_of_concert.month < 10)
		printf(" 0%d 0%d %d ", concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	else if (concert.date_of_concert.day >= 10 && concert.date_of_concert.month < 10)
		printf(" %d 0%d %d ", concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	else
		printf(" %d %d %d ", concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	int whole = concert.date_of_concert.hour;
	float decimal = concert.date_of_concert.hour;
	decimal = (decimal - whole) * 100;
	printf("%d:%f.0", whole, decimal);
}


void initializeIsChosen(Musician** musicians)
{
	int i = 0;
	while (musicians[i] != NULL)
	{
		musicians[i]->isChosen = false;
		i++;
	}
}


// temp until qSort
void bubbleSort(Musician** arr, int size)
{
	int i, j;
	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			MPI* curr = arr[j]->instruments.head;
			MPI* inst = NULL;
			while (curr != NULL)
			{
				if (curr->insId == i)
					inst = curr;
				curr = curr->next;
			}
			curr = arr[j + 1]->instruments.head;
			MPI* inst2 = NULL;
			while (curr != NULL)
			{
				if (curr->insId == i)
					inst2 = curr;
				curr = curr->next;
			}
			if (inst != NULL && inst2 != NULL)
				if (inst->price > inst2->price)
					swap(&arr[j], &arr[j + 1]);
		}
	}
}
void swap(Musician** m1, Musician** m2)
{
	Musician* tmp;

	tmp = *m1;
	*m1 = *m2;
	*m2 = tmp;
}

char* musicanName(char** arr, int* size)
{
	int i = 0;
	char* str;
	int strSize = 0;
	while (arr[i] != NULL)
	{
		int len = strlen(arr[i]);
		strSize += len;
		i++;
	}
	strSize++; // for '\0'
	str = (char*)malloc(sizeof(char) * strSize);
	str = checkAllocation(str);
	i = 0;
	while (arr[i] != NULL)
	{
		strcat(str, arr[i]);
		i++;
	}
	str[strSize - 1] = '\0';
	*size = strSize;
	return str;
}



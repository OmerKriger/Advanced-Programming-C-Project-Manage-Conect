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
}

// hi omer
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


void findMusicianForConcert(MusiciansCollection* Mcollection, int instAmount, Concert concert)
{
	char importance;
	int i;
	char* str;
	CINode* curr = concert.instruments.head;
	while (curr != NULL)
	{
		if (curr->importance == 0)
		{
			for (i = 0; i < Mcollection[curr->inst].logSize; i++)
			{

				if (Mcollection[curr->inst].pMusicians[i]->isChosen == false)
				{
					int len = strlen(Mcollection[curr->inst].pMusicians[i]->name);
					str = (char*)realloc(str, strlen(str) + sizeof(char) * len);
					str = checkAllocation(str);

				}
		
			}

		}
		curr = curr->next;
	}
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



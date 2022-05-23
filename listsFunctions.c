#include "lists.h"
#include "trees.h"

void CreateMPIList(char* token, char* line, InstrumentTree InstTree, MPIList* list)
{
	char seps[] = SEPS;
	int instID, tmp;
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
		appendMPINodeToList(list, node);

		token = strtok(NULL, seps); // get the next instrument
	}
	free(line); // we finished with the this musician and can free the data line
}

CIList createCIList(InstrumentTree tr)
{
	CIList list;
	list.head = list.tail = NULL;
	char* token, seps[] = SEPS2, * name;
	token = strtok(NULL, seps);
	while (token != NULL)
	{
		name = _strdup(token);
		int instID = findInsId(tr, token);
		token = strtok(NULL, seps);
		int amount = atoi(token);
		token = strtok(NULL, seps);
		char importance = atoi(token);
		CINode* node = createCINode(instID, amount, importance, name);
		appendCINodeToList(&list,node);
		token = strtok(NULL, seps);
	}
	return list;
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

CINode* createCINode(int id,int amount,char importance,char* name)
{
	CINode* node = (CINode*)malloc(sizeof(CINode));
	node = checkAllocation(node);
	node->importance = importance;
	node->inst = id;
	node->num = amount;
	node->insName = name;
	node->next = NULL;
	return node;
}

void appendMPINodeToList(MPIList* list, MPI* node) // weird warning need to look
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

void appendCINodeToList(CIList* list, CINode* node)
{
	if (list->head == NULL || list->tail == NULL)
	{
		list->head = list->tail = node;
	}
	else
	{
		list->tail->next = node;
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

MusiciansCollection* BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount)
{
	// a function to build an array of arrays of pionters to musicians.
	MusiciansCollection* musicianCollection = (MusiciansCollection*)malloc(sizeof(MusiciansCollection) * instAmount);
	musicianCollection = checkAllocation(musicianCollection);

	int i = 0;
	musicianCollection = initializeArray(musicianCollection, instAmount);

	while (musicianGroup[i] != NULL) // check how many players for every instrument.
	{
		MPI* curr = musicianGroup[i]->instruments.head;
		while (curr != NULL)
		{
			int collectionInd = curr->insId;

			if (musicianCollection[collectionInd].phySize == musicianCollection[collectionInd].logSize)
			{
				musicianCollection[collectionInd].phySize *= 2;
				Musician** temp = (Musician**)realloc(musicianCollection[collectionInd].pMusicians, sizeof(Musician*) * musicianCollection[collectionInd].phySize);
				musicianCollection[collectionInd].pMusicians = checkAllocation(temp);
			}
			int logSize = musicianCollection[collectionInd].logSize;
			musicianCollection[collectionInd].pMusicians[logSize] = musicianGroup[i];
			musicianCollection[collectionInd].logSize += 1;

			curr = curr->next;
		}
		i++;
	}
	musicianCollection = tightenTheArr(musicianCollection, instAmount);


	return musicianCollection;
}

MusiciansCollection* initializeArray(MusiciansCollection* collection, unsigned short size)
{
	// a function to initialize the values in musicCollection.
	int i;
	Musician** tmp;

	for (i = 0; i < size; i++)
	{
		tmp = (Musician**)malloc(sizeof(Musician*));
		collection[i].pMusicians = checkAllocation(tmp);
		collection[i].logSize = 0;
		collection[i].phySize = 1;
	}
	return collection;
}

MusiciansCollection* tightenTheArr(MusiciansCollection* collection, unsigned short size)
{
	// a function to tighten a given arr
	int i;
	Musician** tmp;

	for (i = 0; i < size; i++)
	{
		tmp = (Musician**)realloc(collection[i].pMusicians, sizeof(Musician*) * collection[i].logSize);
		collection[i].pMusicians = tmp;
	}
	return collection;
}

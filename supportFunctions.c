#include "listsAndSupport.h"
#include "trees.h"

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
				MusPricePerInst* temp = (MusPricePerInst*)realloc(musicianCollection[collectionInd].pMusicians, sizeof(MusPricePerInst) * musicianCollection[collectionInd].phySize); 
				musicianCollection[collectionInd].pMusicians = checkAllocation(temp);
			}
			int logSize = musicianCollection[collectionInd].logSize;
			musicianCollection[collectionInd].pMusicians[logSize].pForMusician = musicianGroup[i];
			musicianCollection[collectionInd].logSize += 1;
			musicianCollection[collectionInd].pMusicians[logSize].price = curr->price;
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
	MusPricePerInst* tmp;

	for (i = 0; i < size; i++)
	{
		tmp = (MusPricePerInst*)malloc(sizeof(MusPricePerInst));
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
	MusPricePerInst* tmp;
	for (i = 0; i < size; i++)
	{
		tmp = (MusPricePerInst*)realloc(collection[i].pMusicians, sizeof(MusPricePerInst) * collection[i].logSize);
		collection[i].pMusicians = checkAllocation(tmp);
	}
	return collection;
}

void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* mCollection)
{
	char* str;
	Concert concert;
	str = getLineOfText();
	while (str != NULL)
	{
		concert = getConcertData(str, InstTree);
		initializeIsChosen(MusiciansGroup);
		reserveMusicians(mCollection, concert);
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
	free(data);
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
			char* tmp = (char*)realloc(str, sizeof(char) * pSize);
			str = checkAllocation(tmp);
		}
		str[lSize] = c;
		lSize++;
		str[lSize] = '\0';
		c = getchar();
	}
	if (str[0] == '\0')
	{
		free(str);
		return NULL;
	}
	pSize = lSize + 1;
	char* tmp = (char*)realloc(str, sizeof(char) * pSize);
	str = checkAllocation(str);
	return str;
}

Date getDate()
{
	int hour, min;
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
	date.hour = (float)hour + min / 60.0;
	return date;
}

void sortMusicianCollection(MusiciansCollection* mCollection, int instAmount)
{
	// sorting all of the musicians arrays in mCollection.
	int i;
	for (i = 0; i < instAmount; i++)
		qsort(mCollection[i].pMusicians, mCollection[i].logSize, sizeof(MusPricePerInst), comparePrices);
}

int comparePrices(MusPricePerInst* elem1, MusPricePerInst* elem2)
{
	// a function that compares two price elements for qsort.
	return (int)(elem1->price - elem2->price);
}

void reserveMusicians(MusiciansCollection* mCollection, Concert concert)
{
	int totalPrice = 0; // save total price of show
	unsigned short fD_lSize = 0, fD_pSize = 1; // fullDetails logic and physic sizes
	char* reservedDetailsForIns = NULL, ** fullDetails = NULL, ** tmp = NULL; // pointers to strings and array of strings
	fullDetails = checkAllocation((char**)malloc(sizeof(char*) * fD_pSize)); // allocate the array for start and check allocate
	bool showReserveFail = false; // flag if show reserve failed or not
	CINode* InsRequired = concert.instruments.head; // set current instruemt required for search
	while (InsRequired != NULL)
	{
		if (fD_lSize == fD_pSize) // if fullDetails array full make it bigger by 2
		{
			fD_pSize *= 2;
			tmp = (char**)realloc(fullDetails, sizeof(char*) * fD_pSize);
			fullDetails = checkAllocation(tmp);
		}
		MusPricePerInst* pMusicians = mCollection[InsRequired->inst].pMusicians; // save the pointer for musicians play for instrument in local pointer 
		int logSize = mCollection[InsRequired->inst].logSize; // the size of pointer array in mCollection in InstID index.
		reservedDetailsForIns = getDetailsOfMusicians(logSize, pMusicians, InsRequired, &totalPrice); // get details of musicians for this show play in the current instruemnt we look for
		if (reservedDetailsForIns == NULL) // check if we found enough musicians and we can continue in the show planning
		{ // if failed
			showReserveFail = true; // mark that planning failed
			freeShowDetails(fullDetails, fD_lSize); // release all the details we kept about musicians for show
			break; // finish the search
		}
		else // if not fail
		{
			fullDetails[fD_lSize] = reservedDetailsForIns;
			fD_lSize++;
			InsRequired = InsRequired->next; // skip to the next instruemnt required
		}
	}
	if (showReserveFail == true)
		printf("Could not find musicians for the concert %s\n", concert.name); // declere that show failed
	else
	{ // print the details about the show
		fullDetails = checkAllocation( (char**)realloc(fullDetails, sizeof(char*) * (fD_lSize)) );
		printConcertsDetails(concert, fullDetails, totalPrice, fD_lSize); // print details and release
	}
}

void freeShowDetails(char** strArr, int size)
{
	for(int i = size-1; 0 <= i; i--)
		free(strArr[i]);
	free(strArr);
}

char* getDetailsOfMusicians(int size, MusPricePerInst* pMusicians, CINode* InsRequired, int* totalPrice)
{
	unsigned short counter = 0, i, d_pSize = 1, d_lSize=1;
	char* strDetails = checkAllocation( (char*)malloc(d_pSize * sizeof(char)));  // allocate basic size for start and check allocate
	strDetails[0] = '\0'; // set the first char for \0 for strcat later
	if (InsRequired->importance == IMPORTANT) // define if start from the start or end of array depend in importance of instrument
		i = size - 1; else i = 0;
	while (counter < InsRequired->num && 0 <= i && i < size) // run in the legal cells in array of musicians and till we found enough musicians
	{
		if (!(pMusicians[i].pForMusician->isChosen))
		{
			getDataOfMusician(InsRequired, pMusicians[i], &counter,&d_lSize, &d_pSize,&strDetails,totalPrice);
		}
		if (InsRequired->importance == IMPORTANT) // skip for next index to check if importance insturment increase to next index if isnt decrease index by 1
			i--; else i++;
	}
	if (counter != InsRequired->num) // mission failed - we have not enough musicians
	{
		free(strDetails); // release what we found
		return NULL; // return NULL for know the mission failed
	}
	return (checkAllocation( realloc(strDetails, sizeof(char) * d_pSize) ) ); // return the pointer to details of musicians we found after realloc titer and check allocations 
}

void getDataOfMusician(CINode* InsRequired, MusPricePerInst musician, unsigned short* counter, unsigned short* d_lSize, unsigned short* d_pSize, char** strDetails, int* totalPrice)
{
	unsigned short len,totalLen;
	musician.pForMusician->isChosen = true; // mark the musician as chosen for show
	(*counter)++; // count the musician in counter
	char* name = musicanName(musician.pForMusician->name, &len); // get the musician name and return the len of his name
	*totalPrice += (int)musician.price; // add price of instruemnt to this musician
	totalLen = (int)(len + countDigits((int)(musician.price)) + strlen(InsRequired->insName) + SPACES); // calculate the totallen of attach string for allocate
	char* attach = checkAllocation((char*)malloc(sizeof(char) * totalLen+1)); // allocate and check allocate
	(void)sprintf(attach, "%s - %s (%d), ", name, InsRequired->insName, (int)musician.price); // make the attach string
	free(name); // release the name of musician we kept
	unifyStrings(strDetails, attach, d_pSize, d_lSize, totalLen);
}

unsigned int countDigits(unsigned int num)
{
	unsigned int count = 1;
	while (num / 10 == 0) {
		num /= 10;
		count++;
	}
	return count;		
}

void unifyStrings(char** dest, char* src, unsigned short* d_pSize, unsigned short* d_lSize, unsigned short s_size)
{
	while (*d_pSize < *d_lSize + s_size) // double the size of dest string till there are enough space for both string
		*d_pSize *= 2;
	char* allocate = (char*)realloc(*dest, sizeof(char) *(*d_pSize)); // allocate
	*dest = checkAllocation(allocate);
	strcat(*dest, src); // unify the strings to one
	*d_lSize = *d_lSize + s_size; // save the new size 
	free(src); // free the src string
}

int searchInstruemtPrice(MPI* node, unsigned short id)
{
	MPI* currNode = node;
	while (currNode != NULL)
	{
		if (currNode->insId == id)
			return (int)currNode->price;
		currNode = currNode->next;
	}
	return NOT_FOUND;
}

void printConcertsDetails(Concert concert, char** details, int price, unsigned short fD_size)
{
	printf("%s %02d %02d %04d ", concert.name, concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	int hour = (int)concert.date_of_concert.hour, minutes = (((int)(concert.date_of_concert.hour * 100)) % 100);
	printf("%02d:%02d: ", hour, minutes);
	for (int i = 0; i < fD_size; i++)
		printf("%s", details[i]);
	printf("\b\b. Total cost: %d.\n", price);
	freeShowDetails(details,fD_size);
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

char* musicanName(char** arr, unsigned short* size)
{

	int i = 0;
	char* str;
	unsigned short int strSize = 0;
	while (arr[i] != NULL)
	{
		int len = (int)strlen(arr[i]);
		strSize = strSize + len + 1; // +1 for space or \0
		i++;
	}
	str = (char*)malloc(sizeof(char) * strSize);
	str = checkAllocation(str);
	str[0] = '\0';
	checkAllocation(&arr[FIRST_NAME]);
	strcpy(str, arr[FIRST_NAME]);
	i = 1;
	while (arr[i] != NULL)
	{
		strcat(str, " "); // space
		strcat(str, arr[i]);
		if (i)
			i++;
	}
	str[strSize - 1] = '\0';
	*size = strSize;
	return str;
}
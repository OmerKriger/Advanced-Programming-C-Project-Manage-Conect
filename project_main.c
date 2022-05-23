// Includes
#include "mainh.h"
#include "trees.h"
#include "lists.h"
#include "files.h"

// functions
void sortMusicianCollection(MusiciansCollection* mCollection, int instAmount);
void bubbleSort(Musician** arr, int size);
void swap(Musician** m1, Musician** m2);
void initializeIsChosen(Musician** musicians);
void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* mCollection);
char* getLineOfText();
Concert getConcertData(char* data, InstrumentTree InstTree);
Date getDate();
void reserveMusicians(MusiciansCollection* mCollection, Concert concert);
void printConcertsDetails(Concert concert,char* fullDetails,int totalPrice);
char* musicanName(char** arr, unsigned short* size);
int searchInstruemtPrice(MPI* node, unsigned short id);
char* getDetailsOfMusicians(int size, Musician** pMusicians, CINode* InsRequired, int* totalPrice, unsigned short* strLen);

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
	MusiciansCollection* musicianCollection = BuildMusiciansCollection(MusiciansGroup, instAmount);
	sortMusicianCollection(musicianCollection, instAmount);
	initializeIsChosen(MusiciansGroup);
	getShow(InstTree, MusiciansGroup, musicianCollection);
}

void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* mCollection)
{
	char* str;
	Concert concert;
	str = getLineOfText();
	while (str != NULL)
	{
		concert = getConcertData(str,InstTree);
		initializeIsChosen(MusiciansGroup);
		reserveMusicians(mCollection,concert);
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
void sortMusicianCollection(MusiciansCollection* mCollection, int instAmount)
{
	int i;
	for (i = 0; i < instAmount; i++)
		bubbleSort(mCollection[i].pMusicians, mCollection[i].logSize);
}
void reserveMusicians(MusiciansCollection* mCollection, Concert concert)
{
	int totalPrice=0, logSize;
	unsigned short strDetailsLen = 1;
	char* reservedDetailsForIns = NULL, * fullDetails = NULL, * tmp;
	fullDetails = (char*)malloc(sizeof(char) * strDetailsLen);
	fullDetails = checkAllocation(fullDetails);
	fullDetails[0] = '\0';
	bool showReserveFail = false;
	CINode* InsRequired = concert.instruments.head;
	while (InsRequired != NULL)
	{
		Musician** pMusicians = mCollection[InsRequired->inst].pMusicians;
		logSize = mCollection[InsRequired->inst].logSize; // the size of pointer array in mCollection in InstID index.
		reservedDetailsForIns = getDetailsOfMusicians(logSize, pMusicians, InsRequired, &totalPrice, &strDetailsLen);
		if (reservedDetailsForIns == NULL)
		{
			free(fullDetails);
			showReserveFail = true;
			break;
		}
		tmp = (char*)realloc(fullDetails, sizeof(char) * strDetailsLen);
		fullDetails = checkAllocation(tmp);
		strcat(fullDetails, reservedDetailsForIns);
		free(reservedDetailsForIns);
		InsRequired = InsRequired->next;
	}
	if (showReserveFail == true)
		printf("Could not find musicians for the concert %s\n", concert.name);
	else
		printConcertsDetails(concert, fullDetails, totalPrice);
}
char* getDetailsOfMusicians(int size, Musician** pMusicians, CINode* InsRequired,int* totalPrice,unsigned short* strLen)
{
	unsigned short int counter = 0,i,len,pSize=1;
	int price;
	char* strDetails,*name,tmp[150],*attach;
	strDetails = (char*)malloc(pSize * sizeof(char));
	strDetails = checkAllocation(strDetails);
	strDetails[0] = '\0';
	if (InsRequired->importance == IMPORTANT)
		i = size - 1; else i = 0;
	while (counter < InsRequired->num && 0 <= i && i < size)
	{
		if (pMusicians[i]->isChosen == false)
		{
			pMusicians[i]->isChosen = true;
			counter++;
			name = musicanName(pMusicians[i]->name, &len);
			price = searchInstruemtPrice(pMusicians[i]->instruments.head, InsRequired->inst);
			*totalPrice += price;
			sprintf(tmp, "%d", price);
			int totalLen = (int)(len + strlen(tmp) + strlen(InsRequired->insName) + SPACES);
			attach = (char*)malloc(sizeof(char) * totalLen);
			attach = checkAllocation(attach);
			(void)sprintf(attach, "%s - %s (%d), ", name, InsRequired->insName, price);
			pSize += totalLen; 
			char* allocate = (char*)realloc(strDetails, sizeof(char)*pSize);
			strDetails = checkAllocation(allocate);
			strcat(strDetails, attach);
			free(attach);
		}
		if (InsRequired->importance == IMPORTANT)
			i--; else i++;
	}
	if (counter != InsRequired->num)
	{
		free(strDetails);
		return NULL;
	}
	*strLen += pSize; 
	return strDetails;
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
void printConcertsDetails(Concert concert,char* details, int price)
{
	printf("%s %02d %02d %04d ", concert.name, concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	int hour = (int)concert.date_of_concert.hour, minutes = (((int)(concert.date_of_concert.hour * 100)) % 100);
	printf("%02d:%02d: ", hour, minutes);
	printf("%s\b\b. ", details); // \b for remove space and ,
	printf("Total cost: %d.\n", price);

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
		if(i)
		i++;
	}
	str[strSize - 1] = '\0';
	*size = strSize;
	return str;
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





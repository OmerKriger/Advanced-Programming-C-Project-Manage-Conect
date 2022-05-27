#include "filesAndSupport.h"
#include "lists.h"
#include "trees.h"
#define MAX_SIZE_OF_LINE 150
#define ONE 1
#define ALL 0
// functions
char* getLineFromFile(FILE* f) // this function get file pointer and get one line from the file
{
	char* lineResult;
	char line[MAX_SIZE_OF_LINE+1];
	if (fgets(line, MAX_SIZE_OF_LINE + 1, f) != NULL) // check if we in EOF
	{
		int len = (int)strlen(line); // check len of string
		if (line[len - 1] == '\n') // replace \n in \0
			line[len - 1] = '\0';
		lineResult = checkAllocation(_strdup(line)); // duplicate the line and check allocation
	}
	else
		return NULL; // if is EOF return EOF
	return lineResult; // return line result
}
bool checkFilePaths(int argc, char** argv, int RequiredFiles) // checking if there are the amount of files name\path required
{
	if (argc - 1 == RequiredFiles)
		return true;
	else
		return false;
}
void checkOpenFile(FILE* f) // check if file opnned correct
{
	if (f == NULL)
	{
		printf("Opening file failed.");
		exit(1);
	}
}

// support functions
MusiciansCollection* BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount) // this function build musicians collections every cell in array there is pointers to musicians list play in specific instrument
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

void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* mCollection) // this function get data struct and check if concerts able 
{
	char* str;
	Concert concert;
	str = getLineOfText(); // get requires from user (NULL = empty line)
	while (str != NULL) // run till str NULL
	{
		concert = getConcertData(str, InstTree); // get concert data
		initializeIsChosen(MusiciansGroup); // mark all musicians for not chosen
		reserveMusicians(mCollection, concert); // check reserve musician for this concert
		str = getLineOfText(); // get next requires for next concert
	}
}

char* getLineOfText() // this function get line from text
{
	short pSize = 1, lSize = 0;
	char* str = checkAllocation((char*)malloc(sizeof(char) * pSize)); // allocate and check allocation
	str[0] = '\0';
	char c = getchar(); // get text char by char
	while (c != '\n') // stop in enter
	{
		if (lSize + 1 == pSize) 
		{
			pSize *= 2;
			str = checkAllocation((char*)realloc(str, sizeof(char) * pSize));
		}
		str[lSize] = c;
		lSize++;
		str[lSize] = '\0';
		c = getchar();
	}
	if (str[0] == '\0') // if is empty line
	{
		free(str); // free the allocate for line
		return NULL; // return NULL
	}
	pSize = lSize + 1;
	str = checkAllocation((char*)realloc(str, sizeof(char) * pSize) ); // reallocate and check allocation
	return str; // return the line
}

Date getDate() // this function get date and time of concert
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
	for (i = 0; i < instAmount; i++) // sorting every array of arrays
		qsort(mCollection[i].pMusicians, mCollection[i].logSize, sizeof(MusPricePerInst), comparePrices); // sorting by qsort
}

int comparePrices(MusPricePerInst* elem1, MusPricePerInst* elem2)
{
	// a function that compares two price elements for qsort.
	return (int)(elem1->price - elem2->price);
}

void reserveMusicians(MusiciansCollection* mCollection, Concert concert) // this function check if show could happen and reserve all the musicians and print in the end
{
	int totalPrice = 0; // save total price of show
	unsigned short fD_lSize = 0, fD_pSize = 1; // fullDetails logic and physic sizes
	char* reservedDetailsForIns = NULL, ** fullDetails = NULL, ** tmp = NULL; // pointers to strings and array of strings
	fullDetails = checkAllocation((char**)malloc(sizeof(char*) * fD_pSize)); // allocate the array for start and check allocate
	bool showReserveFail = false; // flag if show reserve failed or not
	CINode* tmpReq, *InsRequired = concert.instruments.head; // set current instruemt required for search
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
			freeRequires(InsRequired, ALL);
			break; // finish the search
		}
		else // if not fail
		{
			fullDetails[fD_lSize] = reservedDetailsForIns;
			fD_lSize++;
			tmpReq = InsRequired;
			InsRequired = InsRequired->next; // skip to the next instruemnt required
			freeRequires(tmpReq,ONE);
		}
	}
	
	if (showReserveFail == true)
	{
		printf("Could not find musicians for the concert %s\n", concert.name); // declere that show failed
		free(concert.name);
	}
	else
	{ // print the details about the show
		fullDetails = checkAllocation((char**)realloc(fullDetails, sizeof(char*) * (fD_lSize)));
		printConcertsDetails(concert, fullDetails, totalPrice, fD_lSize); // print details and release
	}
}

void freeRequires(CINode* InsRequired,short int deleteFormat)
{
	do
	{
		free(InsRequired->insName);
		InsRequired = InsRequired->next;
	} while (deleteFormat == ALL && InsRequired != NULL);
}

void freeShowDetails(char** strArr, int size) // free show details after print or if show cannot happen
{
	for (int i = size - 1; 0 <= i; i--)
		free(strArr[i]);
	free(strArr);
}

char* getDetailsOfMusicians(int size, MusPricePerInst* pMusicians, CINode* InsRequired, int* totalPrice) // this function get the musicians details for show
{
	unsigned short counter = 0, i, d_pSize = 1, d_lSize = 1;
	char* strDetails = checkAllocation((char*)malloc(d_pSize * sizeof(char)));  // allocate basic size for start and check allocate
	strDetails[0] = '\0'; // set the first char for \0 for strcat later
	if (InsRequired->importance == IMPORTANT) // define if start from the start or end of array depend in importance of instrument
		i = size - 1; else i = 0;
	while (counter < InsRequired->num && 0 <= i && i < size) // run in the legal cells in array of musicians and till we found enough musicians
	{
		if (!(pMusicians[i].pForMusician->isChosen))
		{
			getDataOfMusician(InsRequired, pMusicians[i], &counter, &d_lSize, &d_pSize, &strDetails, totalPrice);
		}
		if (InsRequired->importance == IMPORTANT) // skip for next index to check if importance insturment increase to next index if isnt decrease index by 1
			i--; else i++;
	}
	if (counter != InsRequired->num) // mission failed - we have not enough musicians
	{
		free(strDetails); // release what we found
		return NULL; // return NULL for know the mission failed
	}
	return (checkAllocation(realloc(strDetails, sizeof(char) * d_pSize))); // return the pointer to details of musicians we found after realloc titer and check allocations 
}

void getDataOfMusician(CINode* InsRequired, MusPricePerInst musician, unsigned short* counter, unsigned short* d_lSize, unsigned short* d_pSize, char** strDetails, int* totalPrice)
{ // this funtion get the details of one musician for printing
	unsigned short len, totalLen;
	musician.pForMusician->isChosen = true; // mark the musician as chosen for show
	(*counter)++; // count the musician in counter
	char* name = musicanName(musician.pForMusician->name, &len); // get the musician name and return the len of his name
	*totalPrice += (int)musician.price; // add price of instruemnt to this musician
	totalLen = (int)(len + countDigits((int)(musician.price)) + strlen(InsRequired->insName) + SPACES); // calculate the totallen of attach string for allocate
	char* attach = checkAllocation((char*)malloc(sizeof(char) * totalLen + 1)); // allocate and check allocate
	(void)sprintf(attach, "%s - %s (%d), ", name, InsRequired->insName, (int)musician.price); // make the attach string
	free(name); // release the name of musician we kept
	unifyStrings(strDetails, attach, d_pSize, d_lSize, totalLen); // link the string
}

unsigned int countDigits(unsigned int num) // this function count how many digits in number
{
	unsigned int count = 1;
	while (num / 10 == 0) {
		num /= 10;
		count++;
	}
	return count;
}

void unifyStrings(char** dest, char* src, unsigned short* d_pSize, unsigned short* d_lSize, unsigned short s_size) // this function take two strings and make one string
{
	while (*d_pSize < *d_lSize + s_size) // double the size of dest string till there are enough space for both string
		*d_pSize *= 2;
	char* allocate = (char*)realloc(*dest, sizeof(char) * (*d_pSize)); // allocate
	*dest = checkAllocation(allocate);
	strcat(*dest, src); // unify the strings to one
	*d_lSize = *d_lSize + s_size; // save the new size 
	free(src); // free the src string
}

void printConcertsDetails(Concert concert, char** details, int price, unsigned short fD_size) // this function print all the details of concert
{
	printf("%s %02d %02d %04d ", concert.name, concert.date_of_concert.day, concert.date_of_concert.month, concert.date_of_concert.year);
	int hour = (int)concert.date_of_concert.hour, minutes = (((int)(concert.date_of_concert.hour * 100)) % 100) * 60 / 100;
	printf("%02d:%02d: ", hour, minutes);
	for (int i = 0; i < fD_size; i++)
		printf("%s", details[i]);
	printf("\b\b. Total cost: %d.\n", price);
	free(concert.name);
	freeShowDetails(details, fD_size);
}

void initializeIsChosen(Musician** musicians) // this function mark all the musician as free to be in next concert we check
{
	int i = 0;
	while (musicians[i] != NULL)
	{
		musicians[i]->isChosen = false;
		i++;
	}
}

char* musicanName(char** arr, unsigned short* size) // this function bring from array of full name one string of full name
{

	int i = 0;
	char* str;
	unsigned short int strSize = 0;
	while (arr[i] != NULL) // check the len of full name for allocate
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
	while (arr[i] != NULL) // copy the name to the string
	{
		strcat(str, " "); // space
		strcat(str, arr[i]);
		if (i)
			i++;
	}
	str[strSize - 1] = '\0';
	*size = strSize;
	return str; // return the name
}

void freeMusicianCollection(MusiciansCollection* musicianCollection, int instAmount)
{
	int i;
	MusPricePerInst* curr;
	for (i = instAmount - 1; 0 <= i; i--)
	{
		curr = musicianCollection[i].pMusicians;
		free(curr);
	}
	free(musicianCollection);
}

void freeAll(InstrumentTree instTree, Musician** MusiciansGroup, MusiciansCollection* musicianCollection, int instAmount)
{
	freeInsTree(instTree);
	freeMusiciansGroup(MusiciansGroup);
	freeMusicianCollection(musicianCollection, instAmount);
}
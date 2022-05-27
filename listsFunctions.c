#include "filesAndSupport.h"
#include "trees.h"
#include "lists.h"

void* checkAllocation(void* ptr) // check allocation function // get pointer check if allocated good and return it // if allocation failed exit from the program and print error
{
	if (ptr == NULL) // check if allocation succcess
	{ // failed
		printf("Allocation Error!");
		exit(1); // exit from the program
	}
	else return ptr; // return the pointer
}

void checkSTRtok(char* ptr) // this function check if there are error in strtok for musician details (details isnt in the format)
{
	if (ptr == NULL) // check token
	{
		printf("error in creating musician");
		exit(1); // exit the program
	}
}

void CreateMPIList(char* token, char* line, InstrumentTree InstTree, MPIList* list) // this function create MusicianPriceInstrument List from the details of musicians
{
	char seps[] = SEPS; // seperators and chars we would like to ignore
	int instID, tmp;
	float price;
	while (token != NULL) // run till the string is finished
	{
		instID = findInsId(InstTree, token); // take the name of instruments and search for his id
		token = strtok(NULL, seps); // grap the price the musician take for this instrument
		checkSTRtok(token); // check if grap is good
		if (sscanf(token, "%d", &tmp) != 1) // check if we grapped number
		{ // if isnt number
			printf("ERROR!, in creating MPIList");
			exit(1); // exit the program
		}
		price = (float)tmp; // transfer this number to float
		MPI* node = CreateMPInode(instID, price); // create MPI Node with ID and price we got
		appendMPINodeToList(list, node); // append the MPI Node to the end of list (if we dont have list yet create one and put it in)

		token = strtok(NULL, seps); // get the next instrument
	}
	free(line); // we finished with the this musician and can free the data line
}

CIList createCIList(InstrumentTree tr) // this function create Concert Instruments list (and return it)
{
	CIList list; // local CIList
	list.head = list.tail = NULL; // make it empty
	char* token, seps[] = SEPS2, * name; // define SEPS for strtok
	token = strtok(NULL, seps); // grap the name of instrument
	while (token != NULL)
	{
		name = _strdup(token); // duplicate the name of instrument 
		int instID = findInsId(tr, token); // find the ID of this instrument
		token = strtok(NULL, seps); // grap the amount required of this instrument
		int amount = atoi(token); // convert it from string to int
		token = strtok(NULL, seps); // grap the importance of the instrument (0/1)
		char importance = atoi(token); // transfer it from string to int
		CINode* node = createCINode(instID, amount, importance, name); // create Concert instrument node from the details we got.
		appendCINodeToList(&list,node); // append this to the end of the list.
		token = strtok(NULL, seps); // grap the next name of instrument (if there are no more instruments finish while)
	}
	return list; // return the list
}

MPI* CreateMPInode(int ID, float price) // create the MPI node
{
	MPI* newNode = (MPI*)malloc(sizeof(MPI)); // allocate MPI
	newNode = checkAllocation(newNode); // check allocation
	newNode->insId = ID; // put ID
	newNode->next = newNode->previous = NULL; // set next and previus to null
	newNode->price = price; // put price of instrument
	return newNode; // return the new node
}

CINode* createCINode(int id,int amount,char importance,char* name) // this function create CI Node from details
{
	CINode* node = (CINode*)malloc(sizeof(CINode)); // allocate the CINode
	node = checkAllocation(node); // check allocation
	// set the data
	node->importance = importance; 
	node->inst = id;
	node->num = amount;
	node->insName = name;
	node->next = NULL;
	return node; // return the node
}

void appendMPINodeToList(MPIList* list, MPI* node) // this function get node and list and put the node in the end of the list
{
	if (IS_EMPTY_LIST) // if list empty
	{
		list->head = list->tail = node; // make it head and tail of the list
	}
	else // if inst empty
	{
		list->tail->next = node; // make the node the next of the tail
		node->previous = list->tail; // make the tail the previus of the node
		list->tail = node; // and make the node the new tail
	}
}

void appendCINodeToList(CIList* list, CINode* node) // this function get node and put it in the end of the list
{
	if (IS_EMPTY_LIST) // if is empty list
	{
		list->head = list->tail = node; // make this node to be the head and tail of the list
	}
	else // if isnt empty
	{
		list->tail->next = node; // make the node be the next of the tail
		list->tail = node; // and make the node be new tail
	}
}

int searchInstruemtPrice(MPI* node, unsigned short id) // this function get ID and head node and search the price of instrument with this ID in list
{
	MPI* currNode = node;
	while (currNode != NULL) // run till end or found id and return price
	{
		if (currNode->insId == id) // if id found
			return (int)currNode->price; // return the price of instrument
		currNode = currNode->next; // skip next node
	}
	return NOT_FOUND; // return not found (error)
}

Concert getConcertData(char* data, InstrumentTree InstTree) // this function get data and make concert
{
	Concert concert; // local concert
	char* token, seps[] = SEPS2; // define seperators for searching
	token = strtok(data, seps); // grap concert name
	concert.name = _strdup(token); // copy the name of conert
	concert.date_of_concert = getDate(); // get date of concert
	concert.instruments = createCIList(InstTree); // create list of instrument required to concert
	free(data); // release the requires line from user
	return concert; // return the concert
}
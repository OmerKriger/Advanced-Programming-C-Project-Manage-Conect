#include "trees.h"
#include "lists.h"
#include "filesAndSupport.h"
// Functions:

InstrumentTree BuildInstTree(char* fileName, unsigned short* instAmount) // this function get filename of instruments file and return tree of instruments and amount of instruments
{
	FILE* f = fopen(fileName, "r"); // file opening for reading
	checkOpenFile(f); // check file openned correct
	InstrumentTree tr; // create local tree
	CreateEmptyTree(&tr); // make root null
	char* insturmentName = getLineFromFile(f); // get one instrument from the file.
	unsigned short counterIDs = 0;
	while (insturmentName != NULL) // run till the end of file
	{
		InsertInstrument(&tr, insturmentName, counterIDs); // inserting instrument to the tree
		insturmentName = getLineFromFile(f); // get the name of the next instrument
		counterIDs++; // increase count by one instrument
	}
	*instAmount = counterIDs; // save the amount by parameter
	fclose(f); // file closing
	return tr; // return the tree
}

TreeNode* FindPlaceForInstrument(TreeNode* node, char* instrument) // this function find the place for instruments for insert the son
{
	if (strcmp(node->instrument, instrument) > 0) // check if instrument we look for in left or right
	{ // left
		if (node->left == NULL) // check if is null
			return node; // return the node
		else
			return FindPlaceForInstrument(node->left, instrument); // if isnt null recursive call to seach left son
	}
	else 
	{ // right
		if (node->right == NULL) //  check if is null
			return node;
		else
			return FindPlaceForInstrument(node->right, instrument); // if isnt null recursive call to seach right son
	}
}

void InsertInstrument(InstrumentTree* tr, char* insturment,unsigned short id) // this function get tree and instrument and id
{
	TreeNode* father; 
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode)); // allocate the new node
	newNode = checkAllocation(newNode); // check allocation
	// put details of new node
	newNode->InsId = id;
	newNode->instrument = insturment;
	newNode->left = newNode->right = NULL;
	
	if (isEmptyTree(*tr)) 
	{
		newNode->father = NULL; // mark father null
		tr->root = newNode; // make newNode as root
	}
	else
	{ // if tree isnt empty
		father = FindPlaceForInstrument(tr->root, insturment); // find the father of instrument required
		newNode->father = father; // make new node father the father we found
		if (strcmp(father->instrument, insturment) > 0) // find where put the newNode (left or right)
			father->left = newNode;
		else
			father->right = newNode;
	}
}

int findInsId(InstrumentTree tree, char* instrument)  // cover function for find instrument id
{
	return findInsIdRec(tree.root, instrument); // find instrument recursive call
}

int findInsIdRec(TreeNode* root, char* instrument) // this function find instrument id by recursive call
{
	if (root == NULL) // if root null (stop condition)
		return NOT_FOUND; // not found
	else 
	{                           
		int res = strcmp(instrument,root->instrument);// compare root instrument to instrument to look
		if (SAME_STRING) // found the instrument
			return root->InsId; // return instrument id
		else if (STR1_GREATER) 
			return findInsIdRec(root->right, instrument); // right Rec
		else
			return findInsIdRec(root->left, instrument); // left Rec
	}
}
void CreateEmptyTree(InstrumentTree* tr) // create empty tree // return null in root
{
	tr->root = NULL;
}

bool isEmptyTree(InstrumentTree tr) // check if tree is empty
{
	if (tr.root == NULL)
		return true; // return true if is empty
	else
		return false; // return false if isnt empty
}

Musician** BuildMusiciansGroup(char fileName[], InstrumentTree InstTree) // this function create group of musicians(array of pointers to musicians)
{
	FILE* f = fopen(fileName, "r"); // open the file of the musicians
	checkOpenFile(f);
	int pSize = 1, lSize = 0; // create default sizes for array
	Musician** musicians = (Musician**)malloc(sizeof(Musician*) * (pSize));
	musicians = checkAllocation(musicians);
	musicians[0] = NULL;
	char* line = getLineFromFile(f);
	while (line != NULL) // run till the file is ending
	{
		if (pSize == lSize + 1) // check if there are enough space in the array.
		{
			pSize *= 2;
			Musician** tmp = (Musician**)realloc(musicians, sizeof(Musician*) * (pSize));
			musicians = checkAllocation(tmp);
		}
		musicians[lSize] = CreateMusician(line, InstTree); // create musician data and put it in the array
		lSize++;
		musicians[lSize] = NULL; // put the mark of last musician in array
		line = getLineFromFile(f); // get new line for next round
	}
	pSize = lSize + 1;
	Musician** tmp = (Musician**)realloc(musicians, sizeof(Musician*) * (pSize)); // make the array tight (last cell is NULL for marking end)
	musicians = checkAllocation(tmp);
	return musicians; // return the array of pointers to musician
}

Musician* CreateMusician(char* line, InstrumentTree InstTree)
{
	char seps[] = SEPS, * token = NULL;
	Musician* musician = (Musician*)malloc(sizeof(Musician));
	musician = checkAllocation(musician);
	musician->name = getMusicianName(&token, line, InstTree); // get the name from the string of line and return token the first instrument
	musician->instruments.head = musician->instruments.tail = NULL; // make the list empty
	CreateMPIList(token, line, InstTree, &(musician->instruments)); // create MPI list for this musician
	return musician;
}

char** getMusicianName(char** pToken, char* line, InstrumentTree InstTree) // this function get name of musician from line of text
{
	char seps[] = SEPS, * token = NULL;
	int instrumentID = NOT_FOUND, pSize = 2, lSize = 0;
	char** fullName = checkAllocation( (char**)malloc(sizeof(char*) * pSize) ); // allocate array and check allocation
	token = strtok(line, seps); // grapping the first
	checkSTRtok(token); // check grapping 
	while (lSize <= 1 || instrumentID == NOT_FOUND)
	{
		if (pSize == lSize + 1)
		{
			pSize *= 2;
			fullName = checkAllocation(realloc(fullName, sizeof(char*) * pSize)); // re allocation and check allocation
		}
		char* name = checkAllocation(_strdup(token)); // duplicate the name
		fullName[lSize] = name; 
		lSize++;
		fullName[lSize] = NULL; // mark the end
		token = strtok(NULL, seps); // grapping next name/instrument in line
		checkSTRtok(token);
		if (lSize > 1) 
			instrumentID = findInsId(InstTree, token); // check if the next grapping is instrument
	}
	pSize = lSize + 1;
	fullName = checkAllocation(realloc(fullName, sizeof(char*) * pSize) );
	*pToken = token; // save token for next step
	return fullName; // return array of full name 
}

void freeInsTree(InstrumentTree instTree)
{
	// a function to release instTree.
	freeInsTreeRec(instTree.root);
}

void freeInsTreeRec(TreeNode* root)
{
	// the rec helper function for freeInsTree.
	if (root == NULL)
		return;
	else
	{
		freeInsTreeRec(root->left);
		freeInsTreeRec(root->right);
		free(root->instrument);
		free(root);
	}
}


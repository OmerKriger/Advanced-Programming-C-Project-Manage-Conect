#include "trees.h"
#include "lists.h"
#include "files.h"
// Functions:

InstrumentTree BuildInstTree(char* fileName, unsigned short* instAmount)
{
	FILE* f = fopen(fileName, "r"); // file opening for reading
	checkOpenFile(f);
	InstrumentTree tr;
	CreateEmptyTree(&tr); // make root null
	char* insturmentName = getLineFromFile(f); // get one instrument from the file.
	unsigned short counterIDs = 0;
	while (insturmentName != NULL) // run till the end of file
	{
		InsertInstrument(&tr, insturmentName, counterIDs);
		insturmentName = getLineFromFile(f); // get the name of the next instrument
		counterIDs++;
	}
	*instAmount = counterIDs;
	fclose(f); // file closing
	return tr;
}
TreeNode* FindPlaceForInstrument(TreeNode* node, char* instrument)
{
	if (strcmp(node->instrument, instrument) > 0)
	{
		if (node->left == NULL)
			return node;
		else
			return FindPlaceForInstrument(node->left, instrument);
	}
	else
	{
		if (node->right == NULL)
			return node;
		else
			return FindPlaceForInstrument(node->right, instrument);
	}
}
void InsertInstrument(InstrumentTree* tr, char* insturment,unsigned short id)
{
	TreeNode* father;
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	newNode = checkAllocation(newNode);
	newNode->InsId = id;
	newNode->instrument = insturment;
	newNode->left = newNode->right = NULL;
	
	if (isEmptyTree(*tr))
	{
		newNode->father = NULL;
		tr->root = newNode;
	}
	else
	{
		father = FindPlaceForInstrument(tr->root, insturment);
		newNode->father = father;
		if (strcmp(father->instrument, insturment) > 0)
			father->left = newNode;
		else
			father->right = newNode;
	}
}
int findInsId(InstrumentTree tree, char* instrument)
{
	return findInsIdRec(tree.root, instrument);
}
int findInsIdRec(TreeNode* root, char* instrument) // ABCDEFGHIJKLMNOPQRSTUVWXYZ
{
	if (root == NULL)
		return NOT_FOUND;
	else
	{                           
		int res = strcmp(instrument,root->instrument);
		if (SAME_STRING)
			return root->InsId;
		else if (STR1_GREATER)
			return findInsIdRec(root->right, instrument); // right Rec
		else
			return findInsIdRec(root->left, instrument); // left Rec
	}
}
void CreateEmptyTree(InstrumentTree* tr)
{
	tr->root = NULL;
}
bool isEmptyTree(InstrumentTree tr)
{
	if (tr.root == NULL)
		return true;
	else
		return false;
}
Musician** BuildMusiciansGroup(char fileName[], InstrumentTree InstTree)
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
	makeEmptyList(&(musician->instruments));
	CreateMPIList(token, line, InstTree, &(musician->instruments));
	return musician;
}
char** getMusicianName(char** pToken, char* line, InstrumentTree InstTree) // maybe need get it shorter ??????
{
	char seps[] = SEPS, * token = NULL;
	int instrumentID = NOT_FOUND, pSize = 2, lSize = 0;
	char** fullName = (char**)malloc(sizeof(char*) * pSize);
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
		token = strtok(NULL, seps); // grapping next name/instrument in line
		checkSTRtok(token);
		if (lSize > 1)
			instrumentID = findInsId(InstTree, token); // check if the next grapping is instrument
	}
	pSize = lSize + 1;
	char** tmp = realloc(fullName, sizeof(char*) * pSize);
	fullName = checkAllocation(tmp);
	*pToken = token;
	return fullName;
}

// support functions
void printTreeInorder(InstrumentTree Tr)
{
	printTreeInorderRec(Tr.root);
}
void printTreeInorderRec(TreeNode* root)
{
	if (root == NULL)
		return;
	printTreeInorderRec(root->left);
	printf("| %s (ID:%d) |", root->instrument, root->InsId);
	printTreeInorderRec(root->right);
}


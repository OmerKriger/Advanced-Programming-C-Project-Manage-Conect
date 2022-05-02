#include "trees.h"

// Functions:

InstrumentTree BuildInstTree(char* fileName)
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


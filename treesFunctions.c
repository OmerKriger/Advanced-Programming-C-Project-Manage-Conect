#include "trees.h"

// Functions:
//														IMPORTANT!!!
// -------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- NEED to check getLineFromFile not take \n ----------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------
InstrumentTree BuildInstTree(char* fileName)
{
	FILE* f = fopen(fileName, "r"); // file opening for reading
	checkOpenFile(f);
	InstrumentTree tr;
	CreateEmptyTree(&tr); // make root null
	char* insturmentName = getLineFromFile(f); // get one instrument from the file.
	unsigned short counterIDs = 0;
	while (insturmentName != EOF) // run till there is instruments (file isnt end)
	{
		InsertInstrument(&tr, insturmentName, counterIDs);
		free(insturmentName); // free the instrument string name because we finish with this instrument
		insturmentName = getLineFromFile(f); // get the name of the next instrument
		counterIDs++;
	}
	fclose(f); // file closing
	return tr;
}
TreeNode* FindPlaceForInstrument(TreeNode* node, char* instrument)
{
	if (strcmp(node->instrument, instrument) > 0) // ABC 
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
InsertInstrument(InstrumentTree* tr, char* insturment,unsigned short id)
{
	TreeNode* father;
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	newNode = checkAllocation(newNode);
	newNode->InsId = id;
	newNode->instrument = insturment;
	newNode->left = newNode->right = NULL;
	
	if (isEmptyTree(*tr))
		tr->root = newNode;
	else
	{
		father = FindPlaceForInstrument(tr->root, insturment);
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
int findInsIdRec(TreeNode* root, char* instrument)
{
	if (root == NULL)
		return NOT_FOUND;
	else
	{                           
		int res = strcmp(root->instrument, instrument);
		if (SAME_STRING)
			return root->InsId;
		else if (STR1_GREATER)
			return findInsIdRec(root->left,instrument); // left Rec
		else
			return findInsIdRec(root->right,instrument); // right Rec
	}
}
void printTreeInorder(InstrumentTree Tr)
{
	printTreeInorderRec(Tr.root);
}
void printTreeInorderRec(TreeNode* root)
{
	if (root == NULL)
		return;
	printTreeInorderRec(root->left);
	printf("| %s (ID:%d) |", root->instrument,root->InsId);
	printTreeInorderRec(root->right);
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



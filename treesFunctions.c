#include "trees.h"

#define ROOT_INDEX size/2
#define NOT_FOUND -1
#define SAME_STRING res == 0
#define STR1_GREATER res > 0
#define NORMAL 1
#define OPPOSITE -1

InstrumentTree BuildInstTree(char** InstrumentsList, int ListSize)
{
	InstrumentTree tr;
	int count=0;
	BuildInstTreeRec(&(tr.root), InstrumentsList, ListSize,&count);
	printTreeInorder(tr);
	return tr;
}
void BuildInstTreeRec(TreeNode** root,char** InstrumentsList, int size,int* count)
{
	if (size == 0)
	{
		*root = NULL;
		return;
	}
	else
	{
		TreeNode* tmp = (TreeNode*)malloc(sizeof(TreeNode));
		(*root) = checkAllocation(tmp);
		(*root)->instrument = InstrumentsList[ROOT_INDEX];
		BuildInstTreeRec(&((*root)->left), InstrumentsList, ROOT_INDEX,count); // left rec
		(*root)->InsId = *count;
		(*count)++;
		BuildInstTreeRec(&((*root)->right), InstrumentsList + ROOT_INDEX + 1, (size-1)/2,count); // right rec
	}
}
int findInsId(InstrumentTree tree, char* instrument)
{
	return findInsIdRec(tree.root, instrument,NORMAL);
}
int findInsIdRec(TreeNode* root, char* instrument,int tunning)
{
	if (root == NULL)
		return NOT_FOUND;
	else
	{                           
		int res = strcmp(root->instrument, instrument) * tunning;
		if (SAME_STRING)
			return root->InsId;
		else if (STR1_GREATER)
			return findInsIdRec(root->left,instrument,NORMAL); // left Rec
		else
			return findInsIdRec(root->right,instrument,OPPOSITE); // right Rec
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

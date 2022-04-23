#include "trees.h"

#define ROOT_INDEX size/2

InstrumentTree BuildInstTree(char** InstrumentsList, int ListSize)
{
	InstrumentTree tr;
	BuildInstTreeRec(&(tr.root), InstrumentsList, ListSize);
	printTreeInorder(tr);
	return tr;
}
void BuildInstTreeRec(TreeNode** root,char** InstrumentsList, int size)
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
		BuildInstTreeRec(&((*root)->left), InstrumentsList, ROOT_INDEX); // left rec
		BuildInstTreeRec(&((*root)->right), InstrumentsList + ROOT_INDEX + 1, (size-1)/2); // right rec
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
	printf("%s ", root->instrument);
	printTreeInorderRec(root->right);
}

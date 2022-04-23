#pragma once
// defines
#include "mainh.h"
// structs
typedef struct treeNode {
	char* instrument;
	unsigned short InsId;
	struct treeNode* left;
	struct treeNode* right;
} TreeNode;

typedef struct tree {
	TreeNode* root;
} InstrumentTree;

// functions decleration
int findInsId(InstrumentTree tree, char* instrument);
InstrumentTree BuildInstTree(char** InstrumentsList, int ListSize);
void BuildInstTreeRec(TreeNode** root, char** InstrumentsList, int size);
void printTreeInorder(InstrumentTree Tr);
void printTreeInorderRec(TreeNode* root);

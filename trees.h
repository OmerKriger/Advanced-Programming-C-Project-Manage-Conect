#pragma once
// defines
#include "mainh.h"
#define NOT_FOUND -1
#define SAME_STRING res == 0
#define STR1_GREATER res > 0

// structs
typedef struct treeNode {
	char* instrument;
	unsigned short InsId;
	struct treeNode* father;
	struct treeNode* left;
	struct treeNode* right;
} TreeNode;

typedef struct tree {
	TreeNode* root;
} InstrumentTree;

// functions decleration
int findInsId(InstrumentTree tree, char* instrument);
int findInsIdRec(TreeNode* root, char* instrument);
InstrumentTree BuildInstTree(char* fileName, unsigned short* instAmount);
void InsertInstrument(InstrumentTree* tr, char* insturment, unsigned short id);
void CreateEmptyTree(InstrumentTree* tr);
bool isEmptyTree(InstrumentTree tr);
TreeNode* FindPlaceForInstrument(TreeNode* node, char* instrument);

// test functions
void printTreeInorder(InstrumentTree Tr);
void printTreeInorderRec(TreeNode* root);

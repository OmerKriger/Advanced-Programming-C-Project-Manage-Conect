#include "mainh.h"

/*
Header file for tree functions
*/

// functions decleration
int findInsId(InstrumentTree tree, char* instrument);
int findInsIdRec(TreeNode* root, char* instrument);
InstrumentTree BuildInstTree(char* fileName, unsigned short* instAmount);
void InsertInstrument(InstrumentTree* tr, char* insturment, unsigned short id);
void CreateEmptyTree(InstrumentTree* tr);
bool isEmptyTree(InstrumentTree tr);
TreeNode* FindPlaceForInstrument(TreeNode* node, char* instrument);
Musician* CreateMusician(char* line, InstrumentTree InstTree);
char** getMusicianName(char** pToken, char* line, InstrumentTree InstTree);
Musician** BuildMusiciansGroup(char fileName[], InstrumentTree InstTree);

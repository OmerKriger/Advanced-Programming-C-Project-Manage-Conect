#include "mainh.h"

/*
Header file for lists function
*/

#define IS_EMPTY_LIST (list->head == NULL || list->tail == NULL)
// function declare
MPI* CreateMPInode(int ID, float price);
void CreateMPIList(char* token, char* line, InstrumentTree InstTree, MPIList* list);
void appendMPINodeToList(MPIList* list, MPI* node);
CIList createCIList(InstrumentTree tr);
CINode* createCINode(int id, int amount, char importance, char* name);
void appendCINodeToList(CIList* list, CINode* node);
Concert getConcertData(char* data, InstrumentTree InstTree);
void freeMusiciansGroup(Musician** MusicianGroup);
void freeMPIList(MPIList lst);
void freeFullName(char** fullName);
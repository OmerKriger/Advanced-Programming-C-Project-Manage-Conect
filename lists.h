#include "mainh.h"

// function declare

void makeEmptyList(MPIList* list);
bool isEmptyList(MPIList* list);
MPI* CreateMPInode(int ID, float price);
void CreateMPIList(char* token, char* line, InstrumentTree InstTree, MPIList* list);
void appendNodeToList(MPIList* list, MPI* node);
MusiciansCollection* BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount);
MusiciansCollection* initializeArray(MusiciansCollection* collection, unsigned short size);
MusiciansCollection* tightenTheArr(MusiciansCollection* collection, unsigned short size);
#include "mainh.h"

// function declare

void makeEmptyList(MPIList* list);
bool isEmptyList(MPIList* list);
MPI* CreateMPInode(int ID, float price);
void CreateMPIList(char* token, char* line, InstrumentTree InstTree, MPIList* list);
void appendMPINodeToList(MPIList* list, MPI* node);
CIList createCIList(InstrumentTree tr);
CINode* createCINode(int id, int amount, char importance, char* name);
void appendCINodeToList(CIList* list, CINode* node);
MusiciansCollection* BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount);
MusiciansCollection* initializeArray(MusiciansCollection* collection, unsigned short size);
MusiciansCollection* tightenTheArr(MusiciansCollection* collection, unsigned short size);


void sortMusicianCollection(MusiciansCollection* mCollection, int instAmount);
void initializeIsChosen(Musician** musicians);
void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* mCollection);
char* getLineOfText();
Concert getConcertData(char* data, InstrumentTree InstTree);
Date getDate();
void reserveMusicians(MusiciansCollection* mCollection, Concert concert);
void printConcertsDetails(Concert concert, char* fullDetails, int totalPrice);
char* musicanName(char** arr, unsigned short* size);
int searchInstruemtPrice(MPI* node, unsigned short id);
char* getDetailsOfMusicians(int size, MusPricePerInst* pMusicians, CINode* InsRequired, int* totalPrice, unsigned short* strLen);
int comparePrices(MusPricePerInst* elem1, MusPricePerInst* elem2);

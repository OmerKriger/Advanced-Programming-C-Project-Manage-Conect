#include "mainh.h"

/*
header file for all support declerations and files declerations
*/
// declerations
 
// files
char* getLineFromFile(FILE* f);
void checkOpenFile(FILE* f);
bool checkFilePaths(int argc, char** argv, int RequiredFiles);

// support
char* getLineOfText();
Date getDate();
unsigned int countDigits(unsigned int num);
MusiciansCollection* BuildMusiciansCollection(Musician** musicianGroup, unsigned short instAmount);
MusiciansCollection* initializeArray(MusiciansCollection* collection, unsigned short size);
MusiciansCollection* tightenTheArr(MusiciansCollection* collection, unsigned short size);
void sortMusicianCollection(MusiciansCollection* mCollection, int instAmount);
void initializeIsChosen(Musician** musicians);
void getShow(InstrumentTree InstTree, Musician** MusiciansGroup, MusiciansCollection* mCollection);
void reserveMusicians(MusiciansCollection* mCollection, Concert concert);
void getDataOfMusician(CINode* InsRequired, MusPricePerInst musician, unsigned short* counter, unsigned short* d_lSize, unsigned short* d_pSize, char** strDetails, int* totalPrice);
void unifyStrings(char** dest, char* src, unsigned short* d_pSize, unsigned short* d_lSize, unsigned short s_size);
void printConcertsDetails(Concert concert, char** details, int price, unsigned short fD_size);
char* musicanName(char** arr, unsigned short* size);
int searchInstruemtPrice(MPI* node, unsigned short id);
char* getDetailsOfMusicians(int size, MusPricePerInst* pMusicians, CINode* InsRequired, int* totalPrice);
int comparePrices(MusPricePerInst* elem1, MusPricePerInst* elem2);
void freeShowDetails(char** strArr, int size);
void freeMusicianCollection(MusiciansCollection* musicianCollection, int instAmount);
void freeAll(InstrumentTree instTree, Musician** MusiciansGroup, MusiciansCollection* musicianCollection, int instAmount);
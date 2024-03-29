#include <stdlib.h>

// Includes
#include "mainh.h"
#include "trees.h"
#include "lists.h"
#include "filesAndSupport.h"

void main(int argc, char* argv[]) // the main function get the name of instruments file and musicians file
{	
	unsigned short instAmount;
	if (!checkFilePaths(argc, argv, FILES)) // checking that file name/path is OK and no missing file\path.
	{
		printf("Missing name/paths of files");
		return;
	}
	InstrumentTree InstTree = BuildInstTree(argv[INSTRUMENTS], &instAmount); // this function get the instruments file name and create from the file instruments tree and return the pointer of tree and amount of instruments.
	Musician** MusiciansGroup = BuildMusiciansGroup(argv[MUSICIANS], InstTree); // this function get the file with musicians details and make array of pointers to musicians with all the details about each musician
	MusiciansCollection* musicianCollection = BuildMusiciansCollection(MusiciansGroup, instAmount); // this function make musician collection (array hold in every index uniqe array of pointers to musicians who play in the ID (index) of this instrument
	sortMusicianCollection(musicianCollection, instAmount); // this sorting the musician collection by price (the musician take the lowest price for instrument show in first and the highest in the end)
	getShow(InstTree, MusiciansGroup, musicianCollection);  // this function get from user the details required for the show pick-up musicians by the parameters and print the details if have the musicians for the concert.
 
	freeAll(InstTree, MusiciansGroup, musicianCollection, instAmount);

}



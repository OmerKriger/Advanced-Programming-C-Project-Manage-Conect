// Includes
#include "mainh.h"
#include "trees.h"
#include "listsAndSupport.h"
#include "files.h"

void main(int argc, char* argv[])
{	
	unsigned short instAmount;

	if (!checkFilePaths(argc, argv, FILES))
	{
		printf("Missing name/paths of files");
		return;
	}
	InstrumentTree InstTree = BuildInstTree(argv[INSTRUMENTS], &instAmount);
	Musician** MusiciansGroup = BuildMusiciansGroup(argv[MUSICIANS], InstTree);
	MusiciansCollection* musicianCollection = BuildMusiciansCollection(MusiciansGroup, instAmount);
	sortMusicianCollection(musicianCollection, instAmount);
	getShow(InstTree, MusiciansGroup, musicianCollection);
}




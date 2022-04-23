// Includes
#include "mainh.h"
// Defines

// functions declerations
void help(char*** pInstrumentsList, int* pInstListLogSize);

// functions
void main(int argc, char** argv)
{
	char* InstrumentsList[7] = { "Bass","Drums","Flute","Guitar","Keyboards","Violin","Piano"};
	int instListLogSize = 7;
	// calling function 
	InstrumentTree instrumentTree = BuildInstTree( InstrumentsList, instListLogSize);
	int id = findInsId(instrumentTree, "Keyboard");
	printf("\n\n Keyboard ID: %d \n",id);
}

void help(char*** pInstrumentsList,int* pInstListLogSize)
{
	char** InstrumentsList = (char**)malloc(sizeof(char*));
	InstrumentsList = checkAllocation(InstrumentsList);
	int instListPhySize = 1, instListLogSize = 0;
	int pSize, lSize;
	bool flag = true;
	while (flag == true)
	{
		pSize = 2;
		char* str = (char*)malloc(sizeof(char) * pSize);
		str = checkAllocation(str);
		lSize =  0;
		printf("enter instruemtn(done for finish): ");
		char c = getchar();
		while (c != '\n')
		{
			if (lSize == pSize)
			{
				pSize *= 2;
				char* tmp = (char*)realloc(str, sizeof(char) * pSize + 1);
				str = checkAllocation(tmp);
			}
			str[lSize] = c;
			lSize++;
			c = getchar();
		}
		pSize = lSize+1;
		char* tmp = (char*)realloc(str, sizeof(char) * pSize + 1);
		str = checkAllocation(tmp);
		str[lSize] = '\0';
		if (str[0] == '-' && str[1] == '1')
			flag = false;
		else
		{
			if (instListLogSize == instListPhySize)
			{
				instListPhySize *= 2;
				char* tmp = (char*)realloc(str, sizeof(char) * instListPhySize);
				InstrumentsList = checkAllocation(tmp);
			}
			InstrumentsList[instListLogSize] = str;
			instListLogSize++;
		}
	}
	*pInstrumentsList = InstrumentsList;
	*pInstListLogSize = instListLogSize;
}
void* checkAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Error!");
		exit(1);
	}
	else return ptr;
}
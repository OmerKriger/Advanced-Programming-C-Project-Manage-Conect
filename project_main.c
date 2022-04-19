// Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// Defines

// functions declerations
void* checkAllocation(void* ptr);

// functions
void main(int argc, char** argv)
{
	char** inst = (char**)malloc(sizeof(char*));
	int instListSize = 0;
	char* str = (char*) malloc(sizeof(char)*2);
	str = checkAllocation(str);
	int pSize=2, lSize=1;
	char c = getchar();
	str[0] = c;
	str[1] = '\0';
	lSize++;
	while (strcmp(str, "done"))
	{

		while (c != '\n')
		{
			if (lSize == pSize)
			{
				char* tmp = (char*)realloc(str, sizeof(char)* pSize * 2 + 1);
				str = checkAllocation(tmp);
			}
			str[lSize] = c;
			lSize++;
			c = getchar();
		}
		
	}
}

void* checkAllocation(void* ptr)
{
	if (ptr != NULL)
	{
		printf("Error!");
		exit(1);
	}
	else return ptr;
}
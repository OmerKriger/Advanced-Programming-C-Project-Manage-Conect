#include "mainh.h"
#define MAX_SIZE_OF_LINE 150
// functions
char* getLineFromFile(FILE* f)
{
	char* lineResult;
	char line[MAX_SIZE_OF_LINE+1];
	if (fgets(line, MAX_SIZE_OF_LINE + 1, f) != NULL)
	{
		int len = strlen(line);		
		if (line[len - 1] == '\n')
			line[len - 1] = '\0';
		lineResult = _strdup(line);
		lineResult = checkAllocation(lineResult);
	}
	else
		return NULL;
	return lineResult;
}
bool checkFilePaths(int argc, char** argv, int RequiredFiles)
{
	if (argc - 1 == RequiredFiles)
		return true;
	else
		return false;
}
void checkOpenFile(FILE* f)
{
	if (f == NULL)
	{
		printf("Opening file failed.");
		exit(1);
	}
}


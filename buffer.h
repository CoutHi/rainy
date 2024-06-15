#include <stdio.h>

// Function to clear input buffer
void clearBuffer()
{
	int tmp;
	
	do
	{
		tmp = getchar();
	}
	while (tmp != EOF && tmp != '\n');
}

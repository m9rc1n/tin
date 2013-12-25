#include "tin_library.h"

int create_socket()
{
	int sockd;
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockd == -1)
	{
		perror("Socket creation error");
		exit(1);
	}
	return sockd;
} 

int SampleAddInt(int i1, int i2)
{
    return i1 + i2;
}

// A function doing nothing ;)
void SampleFunction1()
{
    // insert code here
}

// A function always returning zero
int SampleFunction2()
{
    // insert code here
    
    return 0;
}

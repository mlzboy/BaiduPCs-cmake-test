#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("Size of a page in bytes:%ld\n",sysconf(_SC_PAGESIZE));
	printf("Max length of a  hostname:%ld\n",sysconf(_SC_HOST_NAME_MAX));
	printf(" The maximum number of files that a process can have open at any time.:%ld\n",sysconf(_SC_OPEN_MAX));
	printf("  The  number  of  clock  ticks  per  second.:%ld\n",sysconf(_SC_CLK_TCK)); 
	printf("The number of processors currently online .:%ld\n",sysconf(_SC_NPROCESSORS_ONLN)); 
	printf("The number of processors configured..:%ld\n",sysconf(_SC_NPROCESSORS_CONF)); 
	return 0;
}

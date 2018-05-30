#include <stdio.h>
#include <string.h>

int main()
{
	//char a[100] = "china";
	char a[7] = {'c','h','i','n','a','\0','1'};
	//strcpy(a+1,a);
	strcpy(a,a+2);
	printf("=======%s\n",a);
	return 1;
}

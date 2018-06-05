#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
/*
https://github.com/coreutils/coreutils/blob/master/src/csplit.c
coreutils' split in linux seems didn't use mmap
the speed is slow,dd is also.
so i write this code for slice a file quickly
*/

/*
usage: ./split_slice bigfile 0,4000
usage: ./split_slice bigfile 4000,4000
usage: ./split_slice bigfile 8000,4000
*/
int main(int argc, char* argv[])
{
	fprintf(stdout,"input args unit is MBytes\n");
	fprintf(stdout,"./split_slice bigfile offset(MB unit),slice_size(MB unit)\n");
	/*
	all unit is MBytes
	argv[1]:file path
	argv[2]:offset,initial value is 0
	*/
	//char* path = "/media/usb/hj.001";
	long sc_pagesize = sysconf(_SC_PAGESIZE);
	printf("sc_pagesize:%ld\n",sc_pagesize);
	long long int pagesize = (long long int)(sc_pagesize*1000LL*1000LL/2LL);//nearly 4000MBytes
	//long long int pagesize = (long long int)(sc_pagesize*10LL*10LL);//nearly 4000MBytes
	printf("%s\n",argv[1]);
	printf("%s\n",argv[2]);
	char* path = argv[1];
	int offset = atoi(argv[2]);
	//assert(path);
	//assert(offset > 0);
	if (offset < 0)
	{
		fprintf(stderr,"offset value error must 0,or integer!\n");
		return -1;
	}
 	int fd = open(path,O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr,"%s\n",strerror(errno));
		return -1;
	}
	//check bigfile size if offset size if out of file size range then exit	
	struct stat st;
	if (fstat(fd,&st))
	{
		perror("fstat error\n");
		return -2;
	}
	printf("original file size:%lld\n",st.st_size);
	printf("pagesize          :%lld\n",pagesize);
	
	if (st.st_size <= pagesize)
	{
		fprintf(stdout,"100,no need split,original file smaller than split pagesize\n");
		return 100;
	}
	
	int pagecount = st.st_size / pagesize;
	long long int length = st.st_size % pagesize;
	if (length > 0LL)
	{
		pagecount += 1;
		if ((offset+1) != pagecount)
		{
			length = pagesize;
		}
	}


	printf("length:%lld\n",length);
	printf("pagecount:%d\n",pagecount);
	if (offset+1 > pagecount)
	{
		fprintf(stderr,"101,offset value must smaller than %d\n",pagecount);
		return 101;
	}


	void* rmap = mmap(NULL,length,PROT_READ,MAP_SHARED,fd,offset*pagesize);
	if (rmap == (void *)-1)
	{
		perror("mmap failed!\n");
		fprintf(stderr,"%s\n",strerror(errno));
		return -1;
	}
	printf("-----------------\n");
	char* slice_file = (char*)malloc(sizeof(path)/sizeof(char) + 10);
	sprintf(slice_file,"%s.%d",path,offset);
	printf("slice_file:%s\n",slice_file);
	printf("-----------------\n");
	int fdw = open(slice_file,O_CREAT|O_LARGEFILE|O_RDWR,0777);
	if (fdw < 0)
	{
		fprintf(stderr,"%s open failed\n",slice_file);
		return -1;
	}
	if(ftruncate(fdw,length) == -1)
	{
		perror("ftruncate error!\n");
		return -1;
	}
	void* wmap = mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fdw,0);
	if (wmap == (void *)-1)
	{
		perror("wmmap failed!\n");
		return -1;
	}
	memcpy(wmap,rmap,length);
	//TODO:use pthead enhanced write speed
	munmap(wmap,length);
	munmap(rmap,length);
	close(fd);
	close(fdw);
	free(slice_file);
	fprintf(stdout,"slice Success!\n");
	return 0;
}

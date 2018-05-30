#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
这个程序的作用是从html.txt读入文本
查找匹配的字符串
'152******00' || ''
替换为
'152******00'
*/

/*
*/
static int save(char* in,const char* path);
static int save(char* in,const char* path)
{
	char* filepath = "./_html.txt";
	if (path == NULL)
	{
		path = filepath;
	}
	FILE* fp = NULL;
	fp = fopen(path,"w");
	if (fp == NULL)
	{
		printf("%s\n",strerror(errno));
		printf("couldn't open the file\n");
		return 0;
	}
	fprintf(fp,"%s",in);
	fclose(fp);
	return 1;
}


/*
input value is a str
return value is replace times
*/
static int _replace(char* d);







static int _replace(char* d)
{
	//查找匹配串，并将其替换
	char* str  = "'152******00' || ''";
	char* str2 = "'152******00'";
	char* str3 = " || ''";
	char* r = strstr(d,str);
	int count = 0;
	while(r != NULL)
	{
		count += 1;
		printf("count:%d\n",count);
		char* dest = r + strlen(str2);
		char* src = dest +strlen(str3);
		strcpy(dest,src);
		printf("d============\n");
		printf("%s\n",d);
		r = strstr(d,str);
	}
	return count;
}

int main()
{
	FILE* fp = NULL;
	fp = fopen("/home/pi/c_html_text_proc/html.txt","r");
	if (fp == NULL)
	{
		printf("%s\n",strerror(errno));
		printf("couldn't open the file\n");
		return 0;
	}
	fseek(fp,0L,SEEK_END); /* 定位到文件末尾 */
	long flen=ftell(fp); /* 得到文件大小 */
	char* d=(char *)malloc(flen+1); /* 根据文件大小动态分配内存空间 */
	if(d == NULL)
	{
		fclose(fp);
		return 0;
	}
	fseek(fp,0L,SEEK_SET); /* 定位到文件开头 */
	fread(d,flen,1,fp); /* 一次性读取全部文件内容 */
	d[flen] = 0; /* 字符串结束标志 */ 
	printf("======================\n");
	printf("%s",d);
	
	int ret = _replace(d);
	if (ret > 0)
	{
		printf("replace result-----------------------------\n");
		printf("%s\n",d);
		printf("replace times:%d\n",ret);
		save(d,NULL);
		save(d,"./aa.txt");
	}


	return 1;
}

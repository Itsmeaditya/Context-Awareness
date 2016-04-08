#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"com_filter.h"

#define btoa(x) ((x)?"true":"false")

typedef unsigned int (*hash_function)(char*, unsigned int len);

unsigned int RSHash(char *str,unsigned int len);
unsigned int JSHash(char *str,unsigned int len);
unsigned int PJWHash(char *str,unsigned int len);		//	S
unsigned int ELFHash(char *str,unsigned int len);		//	E
unsigned int BKDRHash(char *str,unsigned int len);		//	V
unsigned int SDBMHash(char *str,unsigned int len);		//	E	HASH FUNCTIONS
unsigned int DJBHash(char *str,unsigned int len);		//	R
unsigned int DEKHash(char *str,unsigned int len);		//	A
unsigned int BPHash(char *str,unsigned int len);		//	L
unsigned int FNVHash(char *str,unsigned int len);
unsigned int APHash(char *str,unsigned int len);

void generateBloomFilter(char *filename);

void addString_count(char *key);	

bool deleteString(char *key);	

void InitialLookOfContingFilter(char *filename);

bool checkWord(char *key);	

void ShowArray_count(Filter *f);

/* Hash Functions Definitions*/

unsigned int RSHash(char *str, unsigned int len)
{
   	unsigned int b    = 3;
   	unsigned int a    = 6;
   	unsigned int hash = 0;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
     	 	hash = hash * a + (*str);
      		a    = a * b;
   	}
   	return hash;
}

/* End Of RS Hash Function */



unsigned int JSHash(char* str, unsigned int len)
{
   	unsigned int hash = 4;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash ^= ((hash << 5) + (*str) + (hash >> 2));
   	}

   	return hash;
}
/* End Of JS Hash Function */


unsigned int PJWHash(char* str, unsigned int len)
{
   	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   	const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   	const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   	const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   	unsigned int hash              = 0;
   	unsigned int test              = 0;
   	unsigned int i                 = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash = (hash << OneEighth) + (*str);

      		if((test = hash & HighBits)  != 0)
      		{
         		hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      		}
   	}

   	return hash;
}
/* End Of  P. J. Weinberger Hash Function */


unsigned int ELFHash(char* str, unsigned int len)
{
   	unsigned int hash = 0;
  	 unsigned int x    = 0;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash = (hash << 4) + (*str);
      		if((x = hash & 0xF0000000L) != 0)
      		{
         		hash ^= (x >> 24);
      		}
     		hash &= ~x;
   	}

   	return hash;
}
/* End Of ELF Hash Function */


unsigned int BKDRHash(char* str, unsigned int len)
{
   	unsigned int seed = 3; /* 31 131 1313 13131 131313 etc.. */
   	unsigned int hash = 0;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash = (hash * seed) + (*str);
   	}

   	return hash;
}
/* End Of BKDR Hash Function */



unsigned int SDBMHash(char* str, unsigned int len)
{
   	unsigned int hash = 0;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash = (*str) + (hash << 6) + (hash << 16) - hash;
   	}

   	return hash;
}
/* End Of SDBM Hash Function */



unsigned int DJBHash(char* str, unsigned int len)
{
   	unsigned int hash = 5;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash = ((hash << 5) + hash) + (*str);
   	}

   	return hash;
}
/* End Of DJB Hash Function */


unsigned int DEKHash(char* str, unsigned int len)
{
   	unsigned int hash = len;
   	unsigned int i    = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
   	}
   	return hash;
}
/* End Of DEK Hash Function */


unsigned int BPHash(char* str, unsigned int len)
{
   	unsigned int hash = 0;
   	unsigned int i    = 0;
   	for(i = 0; i < len; str++, i++)
   	{
     		hash = hash << 7 ^ (*str);
   	}

   	return hash;
}
/* End Of BP Hash Function */


unsigned int FNVHash(char* str, unsigned int len)
{
   	const unsigned int fnv_prime = 0x811C9DC5;
   	unsigned int hash      = 0;
   	unsigned int i         = 0;

   	for(i = 0; i < len; str++, i++)
   	{
      		hash *= fnv_prime;
      		hash ^= (*str);
   	}

   	return hash;
}
/* End Of FNV Hash Function */

unsigned int APHash(char* str, unsigned int len)
{
   	unsigned int hash = 0xAAAAAAAA;
   	unsigned int i    = 0;
	
   	for(i = 0; i < len; str++, i++)
   	{
      		hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   	}

   	return hash;
}

/* End Of AP Hash Function */

char str[100];
char key[100];
//float falsePositiveRate;
int noOfHashFunctions=10;
int noOfItems=0;
int m=0;	//these are assumptions
FILE *fp;
Filter *f;

int main(int argc,char **argv)
{
	int option;
	m=atoi(argv[2]);
	int comArray[m];
	generateBloomFilter(argv[1]);
	InitialLookOfContingFilter(argv[1]);
	do
	{
		printf("\nEnter u r option:\n\n");
		printf("	1.Add a word to Filter\n");
		printf("	2.Quary the Filter for a word\n");
		printf("	3.Delete the word from the filter\n");
		//printf("	4.Compressed Bloom filter\n");
		printf("	5.Print the filter\n");
		printf("	6.Exit\n");
		scanf("%d",&option);
		while(option < 1 || option > 6)
		{
			printf("You entered an invalid number. Choose 1,2,3,4 5 and 6 only.\n");
			scanf("%d",&option);
		}
		switch(option)
		{
			case 1:
			{
				printf("Enter the word to add to Filter:\n");
				scanf("%s",key);
				addString_count(key);
				noOfItems++;
				//fp=fopen("filter.txt","a+");
				//fputs(key,fp);
				//fputs("\n",fp);
				break;
			}
			case 2:			 			
			{
				printf("Enter the word to query the Filter:\n");		 				
				scanf("%s",key);
				bool result=false;
				result=checkWord(key);
				printf("\nThe given string queried is:%s\n", btoa(result));
				break;
			}
			case 3:
			{
				printf("Enter the string to delete from the Filter:\n");
				scanf("%s",key);
				bool msg=false;
				msg=deleteString(key);
				printf("\nThe given string deleted is:%s\n", btoa(msg));
				noOfItems--;
				break;
			}
			case 4:
			{
				//printf("Compressed Bloom filter:\n");
				 compressed(f,m);
				break;
			}
			case 5:
			{
				printf("\n\nThe Bloom filter is\n\n");
			 	printf("\n\nNo.Of items in Bloom Filter would be:\t%d\n\n",noOfItems);
				ShowArray_count(f);
				break;
			}
			case 6:	
				break;		 			
		}
	}
		while(option != 6);
}

void ShowArray_count(Filter *f)
{
    FilterDump_count(f, stdout);
    printf("\n");
}

void generateBloomFilter(char *filename)
{
	fp=fopen(filename,"r");
	while(fscanf(fp,"%s",str) == 1)
	{
		noOfItems++;
	}
	
	noOfHashFunctions=(m/noOfItems)*0.7;
	printf("\n\nnoOfHashFunctions=%d\n\n",noOfHashFunctions);
	f=FilterCreate(m);
	ClearFilter(f);
	fclose(fp);
}

void InitialLookOfContingFilter(char *filename)
{
	
	fp=fopen(filename,"r");
	printf("\n");
	printf("\nBloom Filter Initially\n");
	ShowArray_count(f);
	fseek(fp,0,SEEK_SET);
	//printf("\nThe given strings in bloom filter:\n");
	while(fscanf(fp,"%s",str) == 1)
	{
		//printf("\n%s\n",str);
		addString_count(str);
	}
	//printf("\n\nNo.Of items %d\n\n",noOfItems);
	printf("\nAfter the strings have inserted into Filter the filter would be:\n");
	ShowArray_count(f);
}

void addString_count(char *key)
{
	switch(noOfHashFunctions)
	{
		
		case 11:
			FilterSetBit_count(f,APHash(key,strlen(key))%m);
		case 10:
			FilterSetBit_count(f,FNVHash(key,strlen(key))%m);
		case 9:
			FilterSetBit_count(f,BPHash(key,strlen(key))%m);
		case 8:
			FilterSetBit_count(f,DEKHash(key,strlen(key))%m);
		case 7:
			FilterSetBit_count(f,DJBHash(key,strlen(key))%m);
		case 6:
			FilterSetBit_count(f,SDBMHash(key,strlen(key))%m);
		case 5:
			FilterSetBit_count(f,BKDRHash(key,strlen(key))%m);
		case 4:
			FilterSetBit_count(f,ELFHash(key,strlen(key))%m);
		case 3:
			FilterSetBit_count(f,PJWHash(key,strlen(key))%m);
		case 2:
			FilterSetBit_count(f,JSHash(key,strlen(key))%m);	
		case 1:
			FilterSetBit_count(f,RSHash(key,strlen(key))%m);
			break;	
	}
	
}

bool checkWord(char *key)
{
	bool temp = true;
	switch(noOfHashFunctions)
	{
		
		case 11: 
			temp=temp && FilterTestBit(f,APHash(key,strlen(key))%m);
		case 10:
			temp=temp && FilterTestBit(f,FNVHash(key,strlen(key))%m);
		case 9:
			temp=temp && FilterTestBit(f,BPHash(key,strlen(key))%m);
		case 8:
			temp=temp && FilterTestBit(f,DEKHash(key,strlen(key))%m);
		case 7:
			temp=temp && FilterTestBit(f,DJBHash(key,strlen(key))%m);
		case 6:
			temp=temp && FilterTestBit(f,SDBMHash(key,strlen(key))%m);
		case 5:
			temp=temp && FilterTestBit(f,BKDRHash(key,strlen(key))%m);
		case 4:
			temp=temp && FilterTestBit(f,ELFHash(key,strlen(key))%m);
		case 3:
			temp=temp && FilterTestBit(f,PJWHash(key,strlen(key))%m);
		case 2:
			temp=temp && FilterTestBit(f,JSHash(key,strlen(key))%m);	
		case 1:
			temp=temp && FilterTestBit(f,RSHash(key,strlen(key))%m);
			break;	
	}
	return temp;
}

bool deleteString(char *key)
{
	bool temp = true;
	switch(noOfHashFunctions)
	{
		
		case 11: 
			temp=temp && FilterTestBit(f,APHash(key,strlen(key))%m);
		case 10:
			temp=temp && FilterTestBit(f,FNVHash(key,strlen(key))%m);
		case 9:
			temp=temp && FilterTestBit(f,BPHash(key,strlen(key))%m);
		case 8:
			temp=temp && FilterTestBit(f,DEKHash(key,strlen(key))%m);
		case 7:
			temp=temp && FilterTestBit(f,DJBHash(key,strlen(key))%m);
		case 6:
			temp=temp && FilterTestBit(f,SDBMHash(key,strlen(key))%m);
		case 5:
			temp=temp && FilterTestBit(f,BKDRHash(key,strlen(key))%m);
		case 4:
			temp=temp && FilterTestBit(f,ELFHash(key,strlen(key))%m);
		case 3:
			temp=temp && FilterTestBit(f,PJWHash(key,strlen(key))%m);
		case 2:
			temp=temp && FilterTestBit(f,JSHash(key,strlen(key))%m);	
		case 1:
			temp=temp && FilterTestBit(f,RSHash(key,strlen(key))%m);
			break;	
	}
	if(temp)
	{
		switch(noOfHashFunctions)
		{
			
			case 11: 
				deletionOrUnset(f,APHash(key,strlen(key))%m);
			case 10:
				deletionOrUnset(f,FNVHash(key,strlen(key))%m);
			case 9:
				deletionOrUnset(f,BPHash(key,strlen(key))%m);
			case 8:
				deletionOrUnset(f,DEKHash(key,strlen(key))%m);
			case 7:
				deletionOrUnset(f,DJBHash(key,strlen(key))%m);
			case 6:
				deletionOrUnset(f,SDBMHash(key,strlen(key))%m);
			case 5:
				deletionOrUnset(f,BKDRHash(key,strlen(key))%m);
			case 4:
				deletionOrUnset(f,ELFHash(key,strlen(key))%m);
			case 3:
				deletionOrUnset(f,PJWHash(key,strlen(key))%m);
			case 2:
				deletionOrUnset(f,JSHash(key,strlen(key))%m);	
			case 1:
				deletionOrUnset(f,RSHash(key,strlen(key))%m);
				break;	
		}
		return temp;
	}
	
}

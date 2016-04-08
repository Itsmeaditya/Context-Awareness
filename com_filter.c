#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<stdbool.h>
#include"com_filter.h"

#define BYTE 8

/* position of character within character array */
int posOfCharInArray(int bit) 
{
	return ((bit)/BYTE);
}

/* index of a bit containing the character */

int indexOfBitInCharacter(bit) 
{
	return (1 << (BYTE - 1 - ((bit) % BYTE)));
	//return (1 << (BYTE - ((bit) % BYTE)));
}
/*number of characters required to accommodate the bits or size of Bloom filter(No.of characters * BYTE) */

int noOfCharactersRequiredForBits(bits) 
{
return ((((bits) - 1) / BYTE) + 1);
}


struct Filter
{

	unsigned char *bloomFilter;
	unsigned int totalBits;
	unsigned int *counter;
};


Filter *FilterCreate(unsigned int bits)
{
	Filter *f;
	f=(Filter *)malloc(sizeof(Filter));
	//printf("%u\n",f);
	if(f == NULL)
	{
		perror("malloc failed");
	}
	else
	{
		f->totalBits=bits;
		f->bloomFilter = (unsigned char *)malloc(sizeof(unsigned char) * noOfCharactersRequiredForBits(bits)); 
		f->counter = (unsigned int *)malloc(sizeof(unsigned int) * bits);
	}
	if((f->bloomFilter == NULL) && (f->counter == NULL))
	{
		perror("malloc failed");
		free(f);
		f=NULL;
	}
	return(f);
}


void FilterSetBit_count(Filter *f,unsigned int bit)
{
	if(f == NULL)
	{
		return;
	}
	if(f->totalBits <= bit)
	{
		return;
	}
	
	f -> bloomFilter[posOfCharInArray(bit)] |= indexOfBitInCharacter(bit);
	f-> counter[bit] = f->counter[bit]+1;
	if(f->counter[bit] == 0)
	{
		f->counter[bit] = 1;
	}
}
int FilterTestBit(Filter *f,unsigned int bit)
{

	return((f -> bloomFilter[posOfCharInArray(bit)] & indexOfBitInCharacter(bit)) != 0);
}

void ClearFilter(Filter *f)
{
	if(f == NULL)
	{
		return;
	}
	memset((void *)(f->bloomFilter),0,noOfCharactersRequiredForBits(f->totalBits));
}

void FilterDump_count(Filter *f, FILE *outFile)
{
    	int i;

    	if((f == NULL) || (f->totalBits == 0))
    	{
        	return;         
    	}

    	if(outFile == NULL)
    	{
        	outFile = stdout;
    	}
    	printf("\n\nFilter:\n\n");
    	for (i = 0;i < noOfCharactersRequiredForBits(f->totalBits);i++)
    	{
               fprintf(outFile,"%02X ",f->bloomFilter[i]);
    	}
    	printf("\n\n");
    	/*printf("Counter values associated with each bucket or position or bit is given below:\n\n");
    	for(i = 0;i < f->totalBits;i++)
    	{	
    		printf("%u\t",*(f->counter+i));
    	}*/
}

int compressed(Filter *f,int m)
{
	int i;
	int cArray[m];
	int temp[m];
	int count=0;
	for(i = 0;i < f->totalBits;i++)
    {	
    	cArray[i] = *(f->counter+i);
    }
    i++;
    cArray[i] = '\0';
    int j=0;
    for(i = 0;i < f->totalBits;i++)
    {	
    	if(cArray[i] > 0)
    	{
    		if(count > 0)
    		{
    			temp[j++] = count;
    		}
    		temp[j++] = cArray[i];
    		count = 0;
    	}
    	else
    	{
    		count = count + 1;
    	}
    	 
    }
    temp[j++]=count;
    temp[j] = '\0';
    for(i = 0;i < j; i++)
    {
    	printf("%d\t",temp[i]);
    }
}

void deletionOrUnset(Filter *f,unsigned int bit)
{
	if(f == NULL)
	{
		return;
	}
	if(f->totalBits <= bit)
	{
		return;
	}
	f-> counter[bit] = f->counter[bit]-1;
	if(f->counter[bit] <= 0)
	{
		f->bloomFilter[posOfCharInArray(bit)] ^= indexOfBitInCharacter(bit);
	}
}

#ifndef FILTER_H
#define FILTER_H

struct Filter;
typedef struct Filter Filter;

Filter *FilterCreate(unsigned int bits);

void FilterSetBit_count(Filter *f,unsigned int bit);

int FilterTestBit(Filter *f,unsigned int bit);

void ClearFilter(Filter *f);

void FilterDump_count(Filter *f, FILE *outFile);

void deletionOrUnset(Filter *f,unsigned int bit);

#endif

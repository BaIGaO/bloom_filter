/* Copyright (c) 2013 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Bloom_filter_(C)?action=history&offset=20100923154723

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Bloom_filter_(C)?oldid=16893
*/
/*
bloom filter example:
e is a string.
f1=h1(e) = 8  -> set b8 to 1----------------------
f2=h2(e) = 1  -> set b1 to 1--------             |
f3=h3(e) = 6  -> set b6 to 1-------|----------   |
f4=h4(4) = 14 -> set b14 to 1------|---------|---|------------
                                  \|/       \|/ \|/         \|/    
                                ---------------------------------------
                                |0|1|0|0|0|0|1|0|1|0|0|0|0|0|1|0|0|0|0|
                                ---------------------------------------                

k:hash function numbers.
m:bits array size.
n:string length.

The False Positive probability of bloom-filter: p=(1 - e^(kn/m))^k
if (1/2)^k=(0.6185)^(n/m),p value is min.

*/
#ifndef __BLOOM_H__
#define __BLOOM_H__

#include<stdlib.h>
#define CBF_MODE

typedef unsigned int (*hashfunc_t)(const char *);
typedef struct {
	size_t asize; /*bits array size*/
	unsigned char *a; /*bits array in memory*/
	size_t nfuncs;/*functions number*/
	hashfunc_t *funcs;/*hash function pointer array*/
} BLOOM;

BLOOM *bloom_create(size_t size, size_t nfuncs, ...);

int bloom_destroy(BLOOM *bloom);

int bloom_add(BLOOM *bloom, const char *s);

int bloom_check(BLOOM *bloom, const char *s);


#ifdef CBF_MODE /*Counter Bloom-Filter*/

int bloom_del(BLOOM *bloom, const char *s);

#endif // CBF_MODE


#endif


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


#include<stdio.h>
#include<string.h>

#include"bloom.h"

unsigned int sax_hash(const char *key)
{
	unsigned int h=0;

	while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;
    printf("[%s] hash value:%d\n",__FUNCTION__,h);
	return h;
}

unsigned int sdbm_hash(const char *key)
{
	unsigned int h=0;
	while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
    printf("[%s] hash value:%d\n",__FUNCTION__,h);
	return h;
}

unsigned int pjw_hash(char *key)
{
    unsigned long h = 0, g;  
    while (*key) {
        h = (h << 4) + *key++;
        if ((g = (h & 0xF0000000))) {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }
    printf("[%s] hash value:%d\n",__FUNCTION__,h);
    return h;
}

unsigned int lh_strhash(char *c)
{
    unsigned long ret=0;
    long n;
    unsigned long v;
    int r; 

    if ((c == NULL) || (*c == '\0'))
        return(ret);
    /*
    unsigned char b[16]; 
    MD5(c,strlen(c),b); 
    return(b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24)); 
    */ 

    n=0x100;
    while (*c)
    {
        v=n|(*c);
        n+=0x100;
        r= (int)((v>>2)^v)&0x0f;
        ret=(ret<<r)|(ret>>(32-r));
        ret&=0xFFFFFFFFL;
        ret^=v*v;
        c++;
    } 
    printf("[%s] hash value:%d\n",__FUNCTION__,(ret>>16)^ret);
    return((ret>>16)^ret);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char line[1024];
	char *p;
	BLOOM *bloom;
	
	if(argc<2) {
		fprintf(stderr, "ERROR: No word file specified\n");
		return EXIT_FAILURE;
	}

	if(!(bloom=bloom_create(25000000, 4, sax_hash, sdbm_hash,pjw_hash,lh_strhash))) {
		fprintf(stderr, "ERROR: Could not create bloom filter\n");
		return EXIT_FAILURE;
	}

	if(!(fp=fopen(argv[1], "r"))) {
		fprintf(stderr, "ERROR: Could not open file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	while(fgets(line, 1024, fp)) {
		if((p=strchr(line, '\r'))) *p='\0';
		if((p=strchr(line, '\n'))) *p='\0';

		bloom_add(bloom, line);
	}

	fclose(fp);

	while(fgets(line, 1024, stdin)) {
		if((p=strchr(line, '\r'))) *p='\0';
		if((p=strchr(line, '\n'))) *p='\0';

		p=strtok(line, " \t,.;:\r\n?!-/()");
		while(p) {
			if(!bloom_check(bloom, p)) {
				printf("No match for ford \"%s\"\n", p);
			}
            else
            {
                printf("matched!\n");
                bloom_del(bloom,p);
            }
			p=strtok(NULL, " \t,.;:\r\n?!-/()");
		}
	}

	bloom_destroy(bloom);

	return EXIT_SUCCESS;
}

// Chris Mananghaya (cam314)
// CS0449: Lab 6

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>

unsigned long (*compressBound)(unsigned long length);
int (*compress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);
int (*uncompress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);

int main(int argc, char** argv) 
{
	if(argc < 3)
	{
		printf("Error! Too many arguments!");
		return 0;
	}

	void* lib = dlopen("libz.so", RTLD_NOW);

	if(lib == NULL) 
	{
		printf("Error! Could not load library!");
		return 0;
	}	

	// load three functions
	compressBound = dlsym(lib, "compressBound");
	if (compressBound == NULL) 
	{
		printf("Error! CompressBound is null!\n");
		exit(0);
	}
	compress = dlsym(lib, "compress");
	if (compress == NULL)
	{
		printf("Error! Compress is null!\n");
		exit(0);
	}
	uncompress = dlsym(lib, "uncompress");
	if (uncompress == NULL) 
	{
		printf("Error! Uncompress is null!\n");
		exit(0);
	}

	void* uncompressedBuffer;
	unsigned long* compressedSize;
	void* compressedBuffer;

	FILE *f;
	f = fopen(argv[2], "rb+");
	fseek(f, 0, SEEK_END);
	unsigned long size;
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	if(strcmp(argv[1], "-c") == 0) 
	{	
		uncompressedBuffer = malloc(size);
		fread(uncompressedBuffer, sizeof(uncompressedBuffer), 1, f);

		*compressedSize = compressBound(size);
		compressedBuffer = malloc(*compressedSize);

		compress(compressedBuffer, compressedSize, uncompressedBuffer, size);

		fwrite(compressedBuffer, size, (unsigned long)compressedSize, stdout);
	}
	if(strcmp(argv[1], "-d") == 0) 
	{
		fread(&size, sizeof(size), 1, f);
		fread(&compressedSize, sizeof(compressedSize), 1, f);

		compressedBuffer = malloc(*compressedSize);

		fread(compressedBuffer, sizeof(compressedBuffer), 1, f);
		uncompressedBuffer = malloc(size);

		uncompress(uncompressedBuffer, (unsigned long*)size, compressedBuffer, (long unsigned int)compressedSize);

		fwrite(uncompressedBuffer, sizeof(uncompressedBuffer), 1, stdout);
	}
	else
	{
		printf("There's an issue. Oh well. Exiting...");
		return 0;
	}
	free(uncompressedBuffer);
	free(compressedBuffer);

}
#ifndef __MYFSLIB_
#define __MYFSLIB_

#define BLOCK_SIZE 4096
#define FILES 10
#define BLOCKS 160

struct block_struct{
	unsigned char hash[22];
	size_t blockSize;
	int filesUsingThis;
	int blockID;
};

typedef struct block_struct block_t;

struct file_struct{
	char fileName[PATH_MAX];
	struct block_struct* blockArray[16];	
	int fileSize;
	int blocksNum;
};

typedef struct file_struct file_t;

void getHashOfString(const char* input, unsigned char* hash, int size);
int writeNewBlock(const char *buf, size_t size);
int findEmptyBlock();
int findEmptyFile();
int checkExistingFiles(const char* path);
int checkExistingBlocks(unsigned char* hash);
int findRealSize(const char* path);
int checkHash(unsigned char* hash1, unsigned char* hash2, int hashSize);

#endif
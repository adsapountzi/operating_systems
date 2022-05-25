#include <openssl/sha.h>
#include <string.h>
#include <limits.h>
#include <fuse.h>
#include "log.h"
#include "params.h"
#include "myfslib.h"


void getHashOfString(const char* input, unsigned char* hash, int size){

	SHA1((unsigned char*)input, size, hash);
}

int writeNewBlock(const char *buf, size_t size){
    FILE* blockfd;
    char blockName[PATH_MAX];
    int bytesWritten;

	sprintf(blockName, "%s/%d.txt",BB_DATA->realStorage, BB_DATA->blockID);
    log_msg("\tblockname:%s \n", blockName);
    
    blockfd = fopen( blockName, "wb+");
    
    bytesWritten = fwrite(buf, 1,size*sizeof(char), blockfd);
    fflush(blockfd);
    
    log_msg("\treturn of fopen:%x \n", blockfd);
    
    fclose(blockfd);

    return bytesWritten;
}

int checkExistingBlocks(unsigned char* hash){
	int  i;

	for(i=0; i<BLOCKS; i++){
		if(BB_DATA->blocks[i].blockSize == 0){
			continue;
		}
		if (checkHash(BB_DATA->blocks[i].hash, hash, 20)==0){
			return(i);//found
		}	
	}
	return(-1);//not found
}

int checkHash(unsigned char* hash1, unsigned char* hash2, int hashSize){
	int i;

	for (i = 0; i < hashSize; i++){
		if (hash1[i] != hash2[i]){
			return -1;
		}
	}

	return 0;
}

int checkExistingFiles(const char* path){
	int  i;

	for(i=0; i<FILES; i++){
		if (strcmp((char*)BB_DATA->files[i].fileName, (char*)path)==0){
			log_msg("\nfound file: %s", path);
			return(i);//found
		}	
	}
	return(-1);//not found
}

int findEmptyBlock(){
	int  i;

	for(i=0; i<BLOCKS; i++){
		if (BB_DATA->blocks[i].blockSize==0){
			return(i);//found
		}	
	}
	return(-1);//not found
}

int findEmptyFile(){
	int  i;

	for(i=0; i<BLOCKS; i++){
		if (strcmp(BB_DATA->files[i].fileName, "")==0){
			return(i);//found
		}	
	}
	return(-1);//not found
}

int findRealSize(const char* path){
	int  i;

	for(i=0; i<FILES; i++){
		if (strcmp(BB_DATA->files[i].fileName, path)==0){
			return(BB_DATA->files[i].fileSize);//found
		}	
	}
	return 0;

}

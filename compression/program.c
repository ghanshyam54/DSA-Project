#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "huffman.h"
#include "lzw.h"
//#define MAX 100

int main(int argc,char *argv[]){
//this is for huffman compression
	if(strcmp(argv[1],"-c1") == 0){
		if(argc <  3){
			perror("INVALID ARGUNMENT");
			errno = EINVAL;
			return errno;
		}
		char filename[20],outputfile[20];
		strcpy(filename,argv[2]);
		strcpy(outputfile,filename);
		strcat(outputfile,".gzip");
		compression(filename,outputfile);		
	}



// this is for decompress the file and convert it into the original file ussing huffman algorithim
	else if(strcmp(argv[1],"-uc1") == 0){
		if(argc != 4){
			perror("INVALID ARGUNMENT");
			errno = EINVAL;
			return errno;
		}
		int fd,fd1;
		fd=open(argv[2],O_RDONLY);
		if(fd == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}
		
		fd1 = open(argv[3],O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
		if(fd == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}

		uncompress(fd,fd1);

		printf("file uncompressed successfull and uncompress file is\t%s\n ",argv[3]);	

	}


//this is to compress the file using the lzw algorithm
	else if(strcmp(argv[1],"-c2") == 0){

		if(argc < 3){
			perror("invalid argunment");
			errno = EINVAL;
			return errno;
		}
		
		int fd = open(argv[2],O_RDONLY);
		if(fd == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}
		char output_file[15] ;
		strcpy(output_file,argv[2]);
		strcat(output_file,".lzw");
		int fd1 = open(output_file,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
		if(fd1 == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}

		compress(fd,fd1);
		close(fd);
		close(fd1);
		printf("The file is compressed Succesfully and the compress file name is :: \n %s\n",output_file);
	}


//this to uncompress the file usind lzw algoritm
	else if(strcmp(argv[1],"-uc2") == 0){

		if(argc != 4){
			perror("INVALIDE ARGUNMENT");
			errno = EINVAL;
			return errno;
		}
		int fd = open(argv[2],O_RDONLY);
		if(fd == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}
		int fd1 = open(argv[3],O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
		if(fd1 == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}
		decompress(fd,fd1);
		close(fd);
		close(fd1);
		printf("The file is uncompressed Succesfully and the uncompress file name is \t %s",argv[3]);
	}


}


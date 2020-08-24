#define dict_size 4095
#define code_len 12
#define max_val 4094

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>

extern float file_size;
extern float output_size;

typedef struct nodelzw {
	int val;
	int pre_code;
	int ch;
	struct nodelzw *next;
}nodelzw;

typedef struct listlzw{
	nodelzw *head;
	nodelzw *tail;
}listlzw;
typedef struct dictionary{
	int val;
	int pre_code;
	int ch;
}dictionary;
dictionary *dict;

void compress(int fd,int fd1);
//void uncompress(int fd,int fd1);
void initlzw(listlzw *l);
void init_dict(listlzw *l);
void appendlzw(listlzw *l,nodelzw *tmp);
void trav_code(listlzw l);
int search(listlzw *l,int pre_code,int ch);
void write_in_file(int fd1,int pre_code,int char_there);
void add_code(listlzw *l,int pre_code,int ch,int nextcode);
void free_dict(listlzw *l);

//for uncompressing of file
void decompress(int fd,int fd1);
void initdictionary();
int get_code(int fd);
void append_dictionary(int pre_code,int last_char,int val);
int write_string(int fd1,int cur_code);
dictionary *get_node(int val);

#define MAX 100

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

typedef struct node {
	char ch;
	int freq;
	int weight;
	struct node *next,*prev;
	struct node *left, *right;
}node;
typedef struct list{
	node *head,*tail;
}list;

typedef struct store{
	char c;
	char code[16];	
	struct store *next,*prev;
}data;
typedef struct list1{
	data *head, *tail;
}list1;

node *nhead;
list1 l1;
extern float f_size;
extern float f_cmp_size;

int compression(char *filename,char *outputfile);
void init(list *l);
void init1(list1 *l);
void search_append(list **l,char ch,int *count);	
int frequency(int fd,list *l);
void traverse(list l);
void retraverse(list l);
void sort(list *l);
void huffman_tree(list *l,int count);
//void swap(list **l,node *left,node *right);
void weight_node(list *l);
void array_sort(node **p);
void assign_code(node *root,char *str,int pos);
int isLeaf(node *root);
void append(char *str,char ch);
void traverse_code();
void compress();
void writecodetop(int fd1,int count,list1 l1);
void write_code(int fd,int fd1);
void convert_store(char ch ,int fd1);
void bit_write(int bit,int fd1);

//folowing functionn for decompress the zip file
void uncompress(int fd,int fd1);
char *decod_buf(char buf);
int compare(char a[],char b[],int end);
char *ch_code(int buf);

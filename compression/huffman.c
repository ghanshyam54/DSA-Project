#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "huffman.h"

float f_size = 0;
float f_cmp_size = 0;


//compression of file using huffman algorithm
int compression(char *filename,char *outputfile){
	node *p, *ht, *st , st1;
		int fd,fd1,i,j,node_count = 0,m=0;
		char c,ch;
		
		//char filename[20],outputfile[20];
		//strcpy(filename,argv[2]);
		//strcpy(outputfile,filename);
		//strcat(outputfile,".gzip");
		fd=open(filename,O_RDONLY);
		if(fd == -1){
			perror("file not open");
			errno=EINVAL;
			return errno;
		}
		list l;
		init(&l);
/*return the the number of nodes or number of different character is the the list*/
		node_count=frequency(fd,&l);
	
		close(fd);
		
//sort	the node of character in order of frequency
		sort(&l);

//give the weight to the node
		weight_node(&l);
		//traverse(l);

		printf("no of diffent character in file =%d\n",node_count);


/******** creates a huffman tree and whose top node is been pointed by nhead ********/ 

		huffman_tree(&l,node_count);

		//printf("%d\n",nhead->right->weight);

/**** it assign an code for each character using the huffman tree and store it in  the list which is globly declare as l1****/

		init1(&l1); // this inits the list1 in which we will  store the codes of the character
		int pos = 0;
		char str[MAX];	
			
		assign_code(nhead,str,pos);
		//traverse_code(file_name);

		fd = open(filename,O_RDONLY);
		fd1 = open(outputfile,O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
/* writecodetop writes the the nos of character and the character and their respective code at the top of the file*/

		writecodetop(fd1,node_count,l1);
		printf("code writen at top of file\n");


/*read the give file which has to be compress and then convert the character into the the codes and then store this code in form of bit one afer the other in one byte*/

		write_code(fd,fd1);

		close(fd);
		close(fd1);	

		printf("compressed file name:\t%s\n",outputfile);

//to free the list the list l and list l1
		node *free_var;
		free_var = l.head;
		while(free_var){
			l.head = free_var->next;
			free(free_var);
			free_var = l.head;
		}
		data *free_var1 = l1.head;
		while(free_var1){
			l1.head = free_var1->next;
			free(free_var1);
			free_var1 = l1.head;
		}
		printf("compression ratio is = \t%f",f_size/f_cmp_size);
return 0;
}




//to init the list 1
void init(list *l){
	l->head = NULL;
	l->tail = NULL;
}

// to init  the list of ch and codes
void init1(list1 *l ){
	l->head =NULL;
	l->tail = NULL;
}

// to init every node
void init_node(node *l){
	l->next = NULL;
	l->prev = NULL;
	l->left = NULL;
	l->right = NULL;
	l->weight= 0;
}



/*if the charACTER is found it increase its frequency and if not  found it add that character to the list*/
void search_append(list **l,char c,int *count){
	node *temp1, *temp, *tmp;
	int found=0;
	/*if((*l)->head == NULL){
		tmp = (node*)malloc(sizeof(node));
		init_node(tmp);
		tmp -> ch = c;
		tmp -> freq = 1;
		(*count)++;	
		(*l)->head=tmp;
		(*l)->tail=tmp;
	}*/	

	temp1 = (*l)->head;
	while(temp1 != NULL){
		
		if(temp1->ch == c){
			found=1;
			break;
		}
		temp1 = temp1->next;	
	}
	
	if(temp1 == NULL){
		tmp=(*l)->tail;
		temp=(node *)malloc(sizeof(node));
		init_node(temp);
		temp->ch=c;
		temp->next=NULL;
		temp->prev=tmp;
		temp->freq=1;
		tmp->next=temp;
		(*l)->tail=temp;
		(*count)++;
	}
	else if(found == 1){
		temp1->freq=temp1->freq +1;
	}
	
}




// this function provides the frequency of the character kn code and And also the size of the file
int frequency(int fd,list *l){
	int count =0;
	char ch;
	node *tmp=NULL;
	tmp = (node*)malloc(sizeof(node));
	init_node(tmp);
	read(fd,&ch,sizeof(char));
	f_size++;
	tmp -> ch = ch;
	tmp -> freq = 1;
	tmp->next = NULL;
	tmp->prev==NULL;
	count++;	
	l->head=tmp;
	l->tail=tmp;	
	if(l -> head == NULL){
		/*l -> head = tmp;
		tmp -> next = NULL;
		l -> tail = tmp;*/
		printf("malloc is not done");
			exit(1);
	}
	while(read(fd,&ch,sizeof(char))){			
		search_append(&l,ch,&count);		
		f_size++;
	}
return count;
}



void traverse(list l){
	node *temp;
	temp=l.head;
	while(temp){
		printf("%c\t%d\n",temp->ch,temp->freq);
		temp=temp->next;
	}
}



void retraverse(list l){
	node *temp;
	temp=l.tail;
	while(temp){
		printf("%c\t%d\n",temp->ch,temp->freq);
		temp=temp->prev;
	}
}
/*void swap(list **l,node *left,node *right) {
	node *tmp, *tmp2, *tmp3, *tmp4;
	tmp2=right->prev;
	tmp = left->prev;
	tmp3= right->next;
	tmp4 = left->next;
	if (tmp){
 	     tmp->next = right;
 	     right -> prev = tmp;
 	  }
	else{
		(*l)->head = right;
		right->prev = NULL;
	}
	if(right->next == NULL){
		left->next=NULL;
		(*l)->tail = left;
	  }
	else{
		left->next=tmp3;
		left->next->prev=left;
		}
	left->prev = tmp2;
	left->prev->next=left;
	right->next =tmp4;
	right->next->prev=right;
}*/




/*selection sort is done in to put  the character in sequence */
void sort(list *l){
	node *temp, *temp1, *temp2;
	int min,tmp_freq;
	char tmp_ch;
	temp=l->head;
	temp1=temp->next;
	
	while(temp){
		//min=temp->freq;
		temp1=temp->next;
		while(temp1){
			if(temp1->freq < temp->freq){
				tmp_ch=temp1->ch;
				temp1->ch=temp->ch;
				temp->ch=tmp_ch;
				tmp_freq=temp1->freq;
				temp1->freq=temp->freq;
				temp->freq=tmp_freq;			
			}
			temp1 =temp1->next;
		}
		temp=temp->next;
	}
}




// assign the weight to the function which is same as the frequency
void weight_node(list *l){
	node *temp = l->head ;
	while(temp){
		temp->weight = temp->freq;
		temp=temp->next;
	}
}



//sort the array in terms of weight of the tree
void array_sort(node **p){
	int i=0,j=0;
	node *temp;
	
	while(p[i] != NULL){
		j=i+1;
		while(p[j] != NULL){
			if(p[i]->weight > p[j]->weight){
				temp = p[i];
				p[i]=p[j];
				p[j]=temp;
			}
			j++;
		}
		i++;
	}
	/*printf("p[0]->weight= %d,p[0]->ch %c\np[1]->weight= %d,,p[1]->ch %c\np[2]->weight= %d,p[2]->ch %c\n\n",p[0]->weight ,p[0]->ch,p[1]->weight ,p[1]->ch,p[2]->weight ,p[2]->ch);*/		
}






/* This creates the tree in this at the left the smallest weight char node is there AND AT right little higher or equal weight node is therenin this way tree is been produce
*in it first the array is produce of node to which the nodes are assign in ascending order
*then in loop each time the new parent node is produce at it left the he smallest weight char node is there AND AT right little higher or equal weight node is there
*this parent node has to added to the array in place on right node and in sorting of array the sort is done from right node
* in this way loop continues till the parrent node will be the root node 
*/
void huffman_tree(list *l,int count){
	node *head[count +1], *tmp, *temp, **p,*tmp2;
	tmp=l->head;
	int i=0,j=0;
	while(tmp){
		head[i++]=tmp;		
		tmp=tmp->next;
	}
	head[i]=NULL;	
	while(head[j+1] != NULL){
		temp = (node *)malloc(sizeof(node));
		init_node(temp);
		temp -> left=head[j++];		
		temp -> right = head[j];
		temp -> weight= temp->left->weight + temp->right->weight;
		//printf("%c\t%c\t%c\tweight = %d\n",temp->ch,temp->left->ch,temp->right->ch,temp->weight);		
		head[j]=NULL;
		head[j] = temp;		
		temp=NULL;
		nhead=head[j];
		array_sort(&head[j]);	
	}
	//printf("%d\n",nhead->weight);	
}





//check wheather the node is leaf node or not
int isLeaf(node *root){  
	return !(root->left) && !(root->right); 
} 




// it gives the code to the character
void assign_code(node *root,char *str,int pos){
	if(root->left){
		str[pos] = '0';
		assign_code(root->left,str,pos+1);
	}
	if(root->right){
		str[pos] = '1';
		assign_code(root->right,str,pos+1);
	}
	if(isLeaf(root)){
		str[pos] = '\0' ;
		//printf("%c%s\t%d\n",root->ch,str,pos);
		append(str,root->ch);
	}

}




//append the node to list l1 containing ch and their code
void append(char *str,char ch){
	data *tmp, *tmp2;
	tmp =(data *)malloc(sizeof(data));
	tmp->c=ch;
	strcpy(tmp->code,str);
	tmp->next = NULL;
	if(l1.head == NULL){
		l1.head = tmp;
		l1.tail = tmp;
	}
	else {
		tmp2=l1.tail;
		tmp2->next=tmp;
		l1.tail=tmp;
	}
}





void traverse_code(){
	data *tmp;
	tmp = l1.head;
	while(tmp){
		printf("%c%s\n",tmp->c,tmp->code);
		tmp=tmp->next;
	}
}




/* the function writes no of nodes
*  then it writes the character and their codes
*/
void writecodetop(int fd1,int count,list1 l1){
	/*count = 0;
	data *tmp2;
	tmp2 = l1.head;	
	while(tmp2)
		count++;*/
	write(fd1,&count,sizeof(int));
	data *tmp;
	tmp = l1.head;
	while(tmp){
		write(fd1,tmp,sizeof(data));
		tmp= tmp->next;
	}
}




//READS one by one all the character and then convert the char into codes and stores it in one byte and write in the file
void write_code(int fd,int fd1){
	char ch;
	while(read(fd,&ch,sizeof(char))){
		convert_store(ch,fd1);//convert the char in code and save it in bits.
	}
}




//it stores searcg for the char in list of codes and copy the code corresponding to char and then store the code in char using the function bit write  
void convert_store(char ch ,int fd1){

	data *tmp;
	char *code;
	tmp = l1.head;
	while(tmp != NULL){
		if(tmp->c == ch){
			//printf("\n%c",tmp->c);
			code = tmp->code;
				break;
		}
		else
			code = NULL;
		tmp=tmp->next;
	}
	while(*code != '\0'){
		if(*code == '1')
			bit_write(1,fd1);
		else
			bit_write(0,fd1);
		code++;
	}
}





//stores one one bit in the character and will all 8 bits are stored in charater it writes the charcter in the file
void bit_write(int bit,int fd1){
	static int w;
	static char byte;
	char tmp;
	if(bit == 1){
		tmp = 1;
		tmp = tmp << (7-w);
		byte = byte | tmp;
	}
	w++;
	if(w == 8){
		write(fd1,&byte,sizeof(char));
		f_cmp_size++;
		w=0; byte=0; //reset the byte
		return;
	}
	return;	
}










//uncompress the the zip file produce by huffman code
void uncompress(int fd,int fd1){
	
		list1 l1;
		int i=0,count=0;
		data d;
		char buf;
		char* decod_str;
		//printf("working");
		
		read(fd,&count,sizeof(int));
		printf("%d",count);
		while(i<count){
			read(fd,&d,sizeof(data));
			//printf("d->c = %c, d->code %s\n",d.c,d.code);
			append(d.code,d.c);
			i++;
		}

		printf("\n\n\n\n");
		//traverse_code();
/* Read character oneby one and then decode the char and then write it into the file.	
*  decode_buf read the one byte at time and decode the group of bits and connvert it into the chars and store the chars in string and return it .		
*/
		while(read(fd,&buf,sizeof(char))){
			decod_str = decod_buf(buf);
			i=0;
			while(decod_str[i] != '\0'){
				write(fd1,&decod_str[i],sizeof(char));
				//decod_buf++;
				i++;
			}
			
			free(decod_str);
		}
	// to free the list contaning the the codes 
		/*data *free_data = l1.head;
		data *tmp;
		int z=0;
		while(free_data){
			tmp = free_data;
			free(tmp);
			free_data = free_data->next;
			z++;
			printf("%d",z);
		}*/
		
		close(fd);
		close(fd1);
			

}




char *decod_buf(char buf){
	int i =0 ,j = 0	,l;
	static int x = 0;
	static int b = 0;
	char *decode = (char *)malloc(16*sizeof(char)),*str;
	l = (int)buf;   // copy the code into the integer 
	l = l & 0x00FF; 
	l = l<<8-x;
	b = b|l;
	x = x+8;
	data *tmp;
	tmp = l1.head;
	while(tmp){ 		// the loop continues till the code is not over
		str = ch_code(b);
		if(!compare(tmp->code,str,x)){
			decode[j++] = tmp->c;
			l = strlen(tmp->code);
			b = b<<l;
			x =x-l;
			tmp = l1.head;
			//printf("hello");
			if(x == 0)	
				break;
			continue;
		}
		tmp = tmp->next;
		
		if(str != NULL)
			free(str);
		str = NULL;
	}
decode[j]='\0';
return decode;
}



//compares the char code abd if there are equal returns 0
int compare(char a[],char b[],int end){
	b[strlen(a)]='\0';
	b[end]='\0';
	return strcmp(a,b);
}



//convert the 16 bits code store int  into the string
char *ch_code(int buf){
	int i=0,j=0,k=0,l=0;
	char *tmp=(char *)malloc(16*sizeof(char));
	for(i=15;i>=0;i--){
		j = 1<<i;
		k = buf & j;
		if(k == 0)
			tmp[l++] = '0';
		else
			tmp[l++] = '1';
	}
	tmp[l]='\0';
	return tmp;
}

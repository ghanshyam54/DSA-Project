#include "lzw.h"

float file_size = 0;
float output_size = 0;





/*Compressiong of file using lsw is done 
1 . In it first the character are read then eof is there or not is checked
2 . then cur_code is read and then the prevchar and cur_code is search in dict if not found then the previous code is return in file and the   previous code and current code is added in dictionary with val greater than 255 as we have the char whose asci value is upto 255;
3 . if the prev_code &cur_code  found in the dictionary then the previous code is equal to the value of the dictionay node and next element cur_elemt is read from the file and again check if found return val  process continue till the previous code and cur_code is found in file and if not found 2 statement occures.

*/



void compress(int fd,int fd1){

	int pre_code = 0;
	char pre_cod;
	if(read(fd,&pre_code,sizeof(char)) == 0)
		return;
	file_size++;
	//pre_code = (int)pre_cod;
	//printf("%d\n\n\n\n",pre_code);

	int ch=0;
	int index=0;
	int nextcode = 256;
	listlzw l;
	initlzw(&l);		

/**this function assign the dictionary up to the char 255 in the listlzw**/

	init_dict(&l);
	//trav_code(l);	

/**reads the data and convert it into the 12 bit (as the 4095 requires 12 bits) and store it in the file**/
/**search function it the string is repeated if repeated then it reads another charcter and if not same then it writes the the previous code in the file and then it add the new sting to the dictionary **/

	while(read(fd,&ch,sizeof(char))) {
		//printf("%c",ch);
		file_size++;
		if((index = search(&l,pre_code,ch)) != -1){
			pre_code = index;
		}

		else {
			int char_there = 1;
			write_in_file(fd1,pre_code,char_there);

			if(nextcode < dict_size){
				add_code(&l,pre_code,ch,nextcode);
				//printf("%d\n\n",pre_code);
				nextcode++;
			}
			//pre_code = 0;
			pre_code = ch;
		}
	}
/*to put the last char and the left over bits in the file*/
	int char_there = 0;	
	write_in_file(fd1,pre_code,char_there);
	//trav_code(l);
// now at the end the dictionary if free by function free_dict
	free_dict(&l);
	
	printf("compression ratio is : %f\n",file_size/output_size);
}




//to destory the dictionay
void free_dict(listlzw *l){

	nodelzw *temp, *tmp = l->head;
	l->head = NULL;
	l->tail = NULL;
	while(tmp){
		temp = tmp;
		tmp = tmp->next;
		free(temp);
	}
}




void initlzw(listlzw *l){

	l->head = NULL;
	l->tail = NULL;

}




// It makes the dictonary up 255 char
void init_dict(listlzw *l){

	nodelzw *tmp;
	int i=0;

	while(i <= 255){
		tmp =(nodelzw *)malloc(sizeof(nodelzw));
		tmp->val = i;
		tmp->pre_code = -1;
		tmp->ch = i;
		tmp->next = NULL;
		appendlzw(l,tmp);
		i++;
	}
}




//it append the node to the list
void appendlzw(listlzw *l,nodelzw *tmp){

	if(l->head == NULL){
		l->head = tmp;
		l->tail = tmp;
	}

	else{
		l->tail->next = tmp;
		l->tail = tmp;
	}
}




// to print the listlzw formed
		
void trav_code(listlzw l){

	nodelzw *tmp;
	tmp = l.head;

	if(tmp == NULL)
		printf("dictionary not made yet");

	while(tmp){
		//printf("hello man");
		printf("l.val= %d \t l.pre_code=%d\t l.ch%c\n",tmp->val,tmp->pre_code,(char)tmp->ch);
	tmp = tmp->next;
	}

}



/*IT searches the previous string and the new_char in the dictionary*/

int search(listlzw *l,int pre_code,int ch){

	nodelzw *tmp = l->head;
	while(tmp){
		if((tmp->pre_code == pre_code)&&(tmp->ch == ch))
			return tmp->val;
		tmp=tmp->next;
	}

	return -1;	
}




/*
*	The code writes the string_code in 12 bits 
*	first 8 bits is writen in first character and the next 4 remaing bits are writen in the next byte when the next code comes,next_code *	with 4 bits and the left 8 bits in another character in this way the 2 codes are writen in 3 bytes;
*	
*
*/

void write_in_file(int fd1,int ch,int ch_there){

	static int left_bit = 0;
	static int leftover = 0;
	
	if(leftover == 0){
		int tmp = ch>>4;
		write(fd1,&tmp,sizeof(char));
		leftover = 1;
		left_bit = ch & 0xF;
		output_size++;
	}

	else{
		left_bit = (left_bit << 4) | (ch >> 8);
		write(fd1,&left_bit,sizeof(char));
		write(fd1,&ch,sizeof(char));
		leftover = 0;
		left_bit = 0;
		output_size = output_size + 2;
	}
	
	 if((ch_there == 0) && (leftover != 0)){
		int tmp = left_bit << 4;
		write(fd1,&tmp,sizeof(char));
		output_size++;	
	}

}




//add the new code string  in the dictionary in the form of of previous code and char
// new_string == previous_code + ch

void add_code(listlzw *l,int pre_code , int ch,int next_code){

	nodelzw *tmp = (nodelzw *)malloc(sizeof(nodelzw));
	tmp->val = next_code;
	tmp->pre_code = pre_code;
	tmp->ch = ch;
	appendlzw(l,tmp);

}

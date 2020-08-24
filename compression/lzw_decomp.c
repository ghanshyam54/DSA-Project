#include "lzw.h"



//to decompress the file 

//it first take the code from in prev_code and cur_code the file
//then it decode that code and write the code in the file and add the previous code and the first character of the cur code in to the dictionary 


void decompress(int fd,int fd1){
	int next_code = 256;
	int pre_code =0, cur_code = 0 ,first_char=0;	
	//initdictionary();
	dict = (dictionary *)malloc(4096*sizeof(dictionary));

	pre_code = get_code(fd);
	//printf("%c",pre_code);
	write(fd1,&pre_code,sizeof(char));	
	while(cur_code = get_code(fd)){
		if (cur_code >= next_code){                    // when string + character + string + character + string ocures this is exception case
			first_char = write_string(fd1,pre_code);
			write(fd1,&first_char,sizeof(char));
		}
		else
			first_char = write_string(fd1,cur_code);
		if(next_code < 4096)
				append_dictionary(pre_code,first_char,next_code++);
		pre_code = cur_code;				
	}
	//free(dict);
}




//Not usefull 
void initdictionary(){
	int i=0;
	while(i<4096){
		dict[i].pre_code=0;
		dict[i].ch=0;
	}
}





/** get_code convert the 12 bits code and put it into an interger and return the integer **/
/** it firsst 1 char then add it into the integer shift it right by four then get next char and shiftleft it by and add with first_char 
	the last 4 left bits are addeg to integer with reight shifting it by 4 and added a new char to the int and returned the int
**/


int get_code(int fd){
	static int leftover = 0;
	static int left_bits = 0;
	int first =0 ,next = 0,send_code = 0;
	
	if(leftover == 0){
		read(fd,&first,sizeof(char));
		read(fd,&next,sizeof(char));
		send_code = (first <<4 ) | (next >>4);	
		left_bits = next & 0xF;
		//printf("nextcode %c\n",send_code);
		leftover = 1;	
	}
	else{
		read(fd,&first,sizeof(char));
		send_code = (left_bits << 8) | first;
		left_bits = 0;
		leftover = 0;		
		//printf("leftcode %c\n",send_code);	
	}
	return send_code;
}





/**write_string writes the strings in the uncompress file if the code is greater than 255 size and then it returns the first char of the string and whichnis the last char of the previous dictionary string . **/ 

int write_string(int fd1,int cur_code){
	if(cur_code < 256){
		write(fd1,&cur_code,sizeof(char));
		return cur_code;
	}
	else{
		int ch = 0,last_char;
		dictionary *tmp = get_node(cur_code); 
		ch = tmp->ch; 
		last_char =write_string (fd1,tmp->pre_code);
	
		write(fd1,&ch,sizeof(char));
		return last_char;
	}
}




//add new string to dictionary array at the code 

void append_dictionary(int pre_code,int ch,int val){
	dict[val].pre_code = pre_code;
	dict[val].ch = ch;
}





// To get dictionary node

dictionary *get_node(int val){
	dictionary *tmp = &dict[val];
	return tmp;
}

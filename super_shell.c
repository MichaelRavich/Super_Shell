#include <stdio.h> 
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
void encryptFile(char *filename, char *num, char *newfilename);  //the function encrypts files content  
void decryptFile(char *filename, char *num, char *newfilename);  //the function decrypts files content 
void letterFreq(char *file);  //the function prints the three characters with highest percentage distribution
void randomFile(char* num, char* output_file);  //the function creates file and writes randomly characters between 'a' - 'z' (also capital letters)
void compressFile(char* path, char* target);  //the function compresses file into target file 
int cal_length_number(int n);  //subfunction of compressFile that calculates the length of number
void uppercaseByIndex(char* file_read, char* file_write, char* I); //the function changes lower case letter to capital letter in index I
void lowercaseByIndex(char* file_read, char* file_write, char* I); //the function changes capital letter to lower case letter in index I
void printError(char *str); //the function prints error message and exit
void lockCmdForTime(char *X, char *T); //the function locks specific command for T seconds 
int checklocked(char *name_cmd); //the function checks if command is locked

#define MAX 256
typedef struct locked{ //struct for lock command
char name[MAX];  //name of command
double finishLock;  //final time of locked
}Locked;

Locked array[MAX]; //global array that contains commands locked

int main(int argc, char* argv[]){
	int j, t = 0, flag = 0;
	char* args[4];
	char* split;
	char input[MAX];
	pid_t pid;
	char* array_command[8] = {"encryptFile", "decryptFile", "lockCmdForTime",  //extra commands beyond bin
		"letterFreq", "uppercaseByIndex", "lowercaseByIndex", "randomFile",
		"compressFile"}; 

	for (j = 0; j < MAX; j++) //reset array of locked commands to "EMPTY"
		strcpy(array[j].name, "EMPTY");

	while(1){
		flag = 0;
		printf("SuperShell>");  //prompt message
		scanf(" %[^\n]s", input);
		rewind(stdin);
		if(!strcmp(input, "byebye")) exit(0); //0 if equal

		for(j = 0 ; j < 4 ; j++)  //scan line from user
			args[j] = NULL;
			split = strtok(input, " ");   
			while(split!=NULL){      //split to arguments
				args[t++] = split;
				split = strtok(NULL, " ");
					}
					t = 0;

			if (checklocked(args[0]) == 1) continue;   


			for(j=0 ;j<8 ;j++)    //if bin command
				if(!strcmp(args[0], array_command[j])) flag = 1;
					
					

			if (flag == 0){  //duplicate process
				pid=fork(); 	
				if(pid == -1){
					printf("fork failed!");
					exit(1);
				}
				if(pid == 0){ //child process
				execvp(args[0], args);  //bin commands
				printf("\nNot Supported\n");
				exit(1);
				}
				wait(NULL);//parent process
				continue;
			}

			else{    //extra commands (flag = 1)
				
				if (!strcmp(args[0], "lockCmdForTime")){
					lockCmdForTime(args[1],args[2]);
					 continue;}
			
				if (!strcmp(args[0], "encryptFile")) {
					encryptFile(args[1],args[2], args[3]);
					continue;}
				if (!strcmp(args[0], "decryptFile")){
					 decryptFile(args[1],args[2], args[3]);
					continue;}

				if (!strcmp(args[0], "letterFreq")){
					 letterFreq(args[1]);
					 continue;}
				if (!strcmp(args[0], "uppercaseByIndex")){
					 uppercaseByIndex(args[1], args[2], args[3]);
					 continue;}
				if (!strcmp(args[0], "lowercaseByIndex")){
					 lowercaseByIndex(args[1], args[2], args[3]);
					 continue;}
				if (!strcmp(args[0], "randomFile")){
					 randomFile(args[1], args[2]);
					 continue;}
				if (!strcmp(args[0], "compressFile")){
					 compressFile(args[1], args[2]);
					 continue;}
				else{
					 printf("\nNot Supported\n");
					continue;}

		}
		
	}
	

}


void encryptFile(char *filename, char *num, char *newfilename){

	int fd_from, fd_to, rbytes, wbytes, i;
	char buff[MAX];
	//open file to read and write
	fd_from = open(filename, O_RDONLY);
	if (fd_from == -1 ){
	   printError("Error open");
	   return;
	}
	fd_to = open(newfilename, O_WRONLY | O_CREAT, 0777);
	if(fd_to == -1 ){
	   printError("Error open");
		return;
	}
	rbytes = read(fd_from, buff, MAX);
	if (rbytes == -1){
	   printError("Error read");
		return;
	}
	
	while(rbytes > 0){
		for(i = 0; i < rbytes - 1 ; i++){
			buff[i] += atoi(num);
			buff[i] %= MAX;     //add value to character
			}
		wbytes = write(fd_to, buff, rbytes);
		if (wbytes == -1){
		   printError("Error write");
			return;
		}
		if (wbytes != rbytes){
			printError("Bad writing");
			return;
		}
		rbytes = read(fd_from, buff, MAX);
		if (rbytes == -1){
		   printError("Error read");
			return;
		}
	}
	close(fd_from);
	close(fd_to);

}

void decryptFile(char *filename, char *num, char *newfilename){

	int fd_from, fd_to, rbytes, wbytes, i;
	char buff[MAX];
	//open file to read and write
	fd_from = open(filename, O_RDONLY);
	if (fd_from == -1 ){
		 printError("Error open");
		return;
	}
	fd_to = open(newfilename, O_WRONLY | O_CREAT, 0777);
	if (fd_to == -1 ){
		printError("Error open");
		return;
	}
	rbytes = read(fd_from, buff, MAX);
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	while(rbytes > 0){
		for(i = 0; i < rbytes -1 ; i++){
			buff[i] -= atoi(num);
			buff[i] %= MAX; //sub value from character
			}
		wbytes = write(fd_to, buff, rbytes);
		if (wbytes == -1){
			printError("Error write");
			return;
		}
		if (wbytes != rbytes){
			printError("Bad writing");
			return;
		}
		rbytes = read(fd_from, buff, MAX);
		if (rbytes == -1){
		  printError("Error read");
		  return;
		}
	}
	close(fd_from);
	close(fd_to);
}

void lockCmdForTime(char *X, char *T){
	struct timeval t1;
	int i;
	for(i = 0; i < MAX ; i++){
		if(!strcmp(array[i].name, "EMPTY")){  //insert the command to array[i]
			strcpy(array[i].name, X);
			gettimeofday(&t1, NULL);
			array[i].finishLock = (t1.tv_sec + atoi(T)) * 1.0; //finish time in seconds
			return;
		}
	}
}
int checklocked(char *name_cmd){
	int i;
	struct timeval t1;
	double temp;
	
	for(i = 0; i < MAX ; i++){
		if(!strcmp(array[i].name, name_cmd)){
			gettimeofday(&t1, NULL);
			temp  = t1.tv_sec * 1.0;
			if(temp > array[i].finishLock) { //if current time > finish time locked of command, so release from array
				strcpy(array[i].name, "EMPTY");
				return 0;  // no locked
			}
			else {
					printf("The command %s is now locked!\n", name_cmd);
					return 1; // locked
			}
		}
	}
	return 0;  //no locked
}

void letterFreq(char *file){
	int fd, rbytes, arr_count[26] = {0}, first, second, third, indexMin, indexFirst, indexSecond, indexThird, size = 0, i, min;   // a=0, z=25
	char buff[MAX];
	//open file to read
	fd= open(file,O_RDONLY);
	if (fd == -1){
		printError("Error open");
		return;
	}
	rbytes = read(fd, buff, MAX);
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	while(rbytes > 0){
		for (i = 0; i < rbytes -1; i++){   //count the letters to arr_count
			if (buff[i] >= 65 && buff[i] <= 90)
				arr_count[buff[i]-65]++;
			if (buff[i] >= 97 && buff[i] <= 122)
				arr_count[buff[i]-97]++;
		}
		rbytes = read(fd, buff, MAX);
		if (rbytes == -1){
		 	printError("Error read");
			return;
		}
	}
	close(fd);

//algorithm to find three largest number in array
	min = arr_count[0];
	indexMin = 0;
	for (i = 1; i < 26; i++)    //find minimal number in array
		if(arr_count[i] < min){
		 min = arr_count[i];
		 indexMin = i;
    }
    third = first = second = min;
	indexFirst = indexSecond = indexThird = indexMin;
    for (i = 0; i < 26; i++) {
        if (arr_count[i] > first) {
            third = second;
			indexThird = indexSecond;
            second = first;
			indexSecond = indexFirst;
            first = arr_count[i];
			indexFirst = i;
        }
		else if (arr_count[i] > second) {
            third = second;
			indexThird = indexSecond;
            second = arr_count[i];
			indexSecond = i;
        }

        else if (arr_count[i] > third){
            third = arr_count[i];
			indexThird = i;
		}
    }
	for( i = 0; i < 26; i++)    //calculate amount of chars between 'a' - 'z' in file
		size += arr_count[i];
	
	printf("%c - %.2f%\n%c - %.2f%\n%c - %.2f%\n", indexFirst+97,((float)arr_count[indexFirst]/size*100), indexSecond+97,((float)arr_count[indexSecond]/size*100), indexThird+97, ((float)arr_count[indexThird]/size*100));
	if(indexFirst+97 == 'e' && indexSecond+97 == 'a' && (indexThird+97 == 'o' || indexThird+97 == 'i'))
		printf("Good Letter Frequency\n");
}

void uppercaseByIndex(char* file_read, char* file_write, char* I){
	int fd_from,fd_to, rbytes, wbytes, i, j = 0, size = 0;
	char buff[MAX];
	char *buff1;
	//open file to read and write
	fd_from = open(file_read,O_RDONLY);
	if (fd_from == -1){
		printError("Error open");
		return;
	}
	fd_to = open(file_write,O_WRONLY|  O_TRUNC | O_CREAT, 0777);
	if (fd_from == -1){
		printError("Error open");
		return;
	}
    rbytes = read(fd_from, buff, MAX);
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	while(rbytes > 0){ //calculate the size of file 
		size += rbytes;
		rbytes = read(fd_from, buff, MAX);
		if (rbytes == -1){
			printError("Error read");
			return;
		}
	}
	lseek(fd_from, 0, SEEK_SET); //pointer to start of file
	buff1 = (char*)malloc(sizeof(char)*size); //allocation memory
	if(buff1 == NULL){
		printError("fail allocation");
		return;
	}
	rbytes = read(fd_from, buff1, size); //buff1 contains the file_read 
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	for(i = 0; i < size; i++){
		if(buff1[i] == ' ') j = 0;
		else{ if(j == atoi(I)){
			    if(buff1[i] >= 97 && buff1[i] <= 122)
				   buff1[i] -= 32;  //change to capital letter
		      }
		     j++;
		}
	}
	wbytes = write(fd_to, buff1, size); //write the result to file_write
	if(wbytes == -1 || wbytes < size){
		printError("Error write");
		return;
	}
	close(fd_from);
	close(fd_to);
	free(buff1);
}

void lowercaseByIndex(char* file_read, char* file_write, char* I){
	int fd_from,fd_to, rbytes, wbytes, i, j = 0, size = 0;
	char buff[MAX];
	char *buff1;
		//open file to read and write
	fd_from = open(file_read,O_RDONLY);
	if (fd_from == -1){
		printError("Error open");
		return;
	}
	fd_to = open(file_write,O_WRONLY|  O_TRUNC | O_CREAT, 0777);
	if (fd_from == -1){
		printError("Error open");
		return;
	}
    rbytes = read(fd_from, buff, MAX);
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	while(rbytes > 0){ //calculate the size of file
		size += rbytes;
		rbytes = read(fd_from, buff, MAX);
		if (rbytes == -1){
			printError("Error read");
			return;
		}
	}
	lseek(fd_from, 0, SEEK_SET); //pointer to start of file
	buff1 = (char*)malloc(sizeof(char)*size); //allocation memory
	if(buff1 == NULL){
		printError("fail allocation");
		return;
	}
	rbytes = read(fd_from, buff1, size); //buff1 contains the file_read 
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	for(i = 0; i < size; i++){
		if(buff1[i] == ' ') j = 0;
		else{ if(j == atoi(I)){
				 if(buff1[i] >= 65 && buff1[i] <= 90)
			     	buff1[i] += 32; //change to lower case letter
		     }
		j++;
		}
	}
	wbytes = write(fd_to, buff1, size); //write the result to file_write
	if(wbytes == -1 || wbytes < size){
		printError("Error write");
		return;
	}
	close(fd_from);
	close(fd_to);
	free(buff1);
}

void randomFile(char* num, char* output_file){
	char *buff;
	int i, flag, wbytes, fd_to;
	//open file to write
	fd_to = open(output_file, O_WRONLY|  O_TRUNC | O_CREAT, 0777);
	if(fd_to == -1){
		printError("Error open");
		return;
	}
	buff = (char*)malloc(sizeof(char)*(atoi(num)+1)); //allocation memory
	if(buff == NULL){
		printError("fail allocation");
		return;
	}
	srand(time(0)); 
	for( i = 0; i < atoi(num); i++){
		flag = random()%2;  //raffle lower case/capital letter 
		if(flag == 0) buff[i]= 'a' + (random() % 26); //lower case letter
		else buff[i] = 'A'+ (random() % 26);  //capital letter
	}
	buff[i] = '\n';
	wbytes = write(fd_to, buff, atoi(num) + 1); //write to output_file
	if(wbytes == -1 || wbytes < atoi(num) + 1){
		printError("Error write");
		return;
	}
	close(fd_to);
	free(buff);
}

void compressFile(char* path, char* target){
	int fd_from,fd_to, rbytes, wbytes, size = 0, i, count = 1, j=0, l_count, t;
	char buff[MAX];
	char *buff1, *buff2;
	char *l;
	//open file to read and write
	fd_from = open(path,O_RDONLY);
	if (fd_from == -1){
		printError("Error open");
		return;
	}
	fd_to = open(target,O_WRONLY|  O_TRUNC | O_CREAT, 0777);
	if (fd_from == -1){
		printError("Error open");
		return;
	}
    rbytes = read(fd_from, buff, MAX);
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	while(rbytes > 0){ //count the size of the read file
		size += rbytes - 1;
		rbytes = read(fd_from, buff, MAX);
		if (rbytes == -1){
			printError("Error read");
			return;
		}
	}
	lseek(fd_from, 0, SEEK_SET); //pointer to start of file
	buff1 = (char*)malloc(sizeof(char)*(size+1)); //allocation memory
	if(buff1 == NULL){
		printError("fail allocation");
		return;
	}
	buff2 = (char*)malloc(sizeof(char)*size); //allocation memory
	if(buff2 == NULL){
		printError("fail allocation");
		return;
	}
	rbytes = read(fd_from, buff1, size); //buff1 contains the read file
	if (rbytes == -1){
		printError("Error read");
		return;
	}
	buff1[size] = '\0';   //indicate end of buffer
	for(i = 0 ; i < size; i++){
		if(buff1[i] == buff1[i+1])
			count++;
		else { if(count > 3) {
				buff2[j++] = buff1[i];
				l_count = cal_length_number(count);
				l = (char*)malloc(sizeof(char)*(l_count+1)); //allocation memory for number of times that the letter appears 
				if (l == NULL){
					printError("fail allocation");
					return;
				}
				sprintf(l, "%d", count);  //the number of times that the letter appears in sequence
				for(t = 0; t < l_count; t++)
					buff2[j++] = l[t];  //put in buff2 the number of times that the letter appears 
				count = 1;
				}
			else{ for(t = 0 ; t < count ; t++)
					buff2[j++] = buff1[i];  //put in buff2 the letters that appears in the file less than 4 times
				count = 1;
				}
			 }
	}

	wbytes = write(fd_to, buff2, j);
	if(wbytes < j ) {
		printError("Bad writing");
		return;
	}
	if (wbytes == -1){
	    printError("Error write");
		return;
	}
	close(fd_from);
	close(fd_to);
	free(buff1);
	free(buff2);
	free(l);
}


int cal_length_number(int n){
	int count = 0;
	while (n != 0) {
        n /= 10;     // n = n/10
        count++;
    }
	return count;
}

void printError(char *str){
	printf("%s\ncommand failed!\n", str);

}


















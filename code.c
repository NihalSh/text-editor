#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FLUSH while(getchar() != '\n')

int shift(FILE* fid, unsigned long len, size_t pos, char direction){
	size_t copyPos;
	size_t pastePos;
	int c;
	if (direction == 0) {
		fseek(fid, -1, SEEK_END);
		copyPos = ftell(fid);
		pastePos = copyPos + len;
		printf("%lu %lu %lu\n", pastePos, copyPos, len);
	} else{
		fseek(fid, pos, SEEK_SET);
		pastePos = ftell(fid);
		copyPos = pastePos + (size_t)len;
		printf("%lu %lu %lu\n", pastePos, copyPos, len);
	}

	while(copyPos >= pos){
		fseek(fid, copyPos, SEEK_SET);
		c = fgetc(fid);
		fseek(fid, pastePos, SEEK_SET);
		fputc(c, fid);
		if (direction == 0) {
			copyPos--;
			pastePos--;
		} else {
			fseek(fid, 0, SEEK_END);
			if (ftell(fid) - 1 > copyPos){
				copyPos++;
				pastePos++;
			} else {
				break;
			}
		}
	}
	return 0;
}

int paste(FILE* fid, char* str, long len, size_t pos){
	shift(fid, len, pos, 0);
	fseek(fid, pos, SEEK_SET);
	fputs(str, fid);
	return 0;
}

int delete(FILE** fid, long len, size_t pos){
	int endPos;
	shift(*fid, len, pos, 1);
	fseek(*fid, -1*len, SEEK_END);
	endPos = ftell(*fid);
	rewind(*fid);
	FILE * newfile = fopen("temp", "w");
	while (ftell(*fid) < endPos){
		fprintf(newfile, "%c", fgetc(*fid));
	}
	fclose(newfile);
	newfile = fopen("temp", "r");
	fseek(newfile, 0, SEEK_END);
	endPos = ftell(newfile);
	rewind(newfile);
	/*No other function offers direct deleting, so manual writing to temp file then writing that back to original file*/
	*fid = freopen(NULL, "w+", *fid);
	while (ftell(newfile) < endPos){
		printf("X\n");
		fprintf(*fid, "%c", fgetc(newfile));
	}
	*fid = freopen(NULL, "r+", *fid);
	fclose(newfile);
	return 0;
}

int display(FILE* fid){
	int c;
	rewind(fid);
	printf("\"");
	while((c = fgetc(fid)) != EOF){
		printf("%c", c);
	}
	printf("\"\n");
	rewind(fid);
	return 0;
}

int copy(FILE* fid, long len, size_t cPos, size_t pPos){
	int count = 0;
	char * str = NULL;
	fseek(fid, cPos, SEEK_SET);
	while(count < len){
		str = (char *)realloc(str, sizeof(char)*(count+1));
		str[count] = fgetc(fid);
		count++;
	}
	str = (char *)realloc(str, sizeof(char)*(count+1));
	str[count] = '\0';
	paste(fid, str, len, pPos);
	free(str);
	return 0;
}

int cut(FILE* fid, long len, size_t cPos, size_t pPos){
	if (cPos > pPos){
	/*original cut position is altered*/
	}else{
		
	}
	return 0;
}

int openFile(FILE** fid, char * filename){
	*fid = fopen(filename, "r+");
	if(fid != NULL){
		return 0;
	}else{
		return 1;
	}
};

int main(){
	int op;
	FILE* fid = NULL;
/*	FILE* fid = fopen("file1", "r+");*/
	char str[50];
	long int pos;
	size_t len;
	printf("Text Editor\n");
	/*There is no save operation because all the operations are done on the file, that is file is not loaded to memory*/
	printf("Options:\n\t1 Open File\n\t2 Save As\n\t3 Close File\n\t4 Copy Text\n\t5 Cut Text\n\t6 Delete Text\n\t7 Save and Quit\n\nOption: ");
	while((op = getchar()) != '7'){
		switch(op){
			case '1':
				if(fid == NULL){
					printf("\tFilename: ");
					scanf(" %s", &str);
					fid = fopen(str, "r+");
					if(fid != NULL){
						printf("\tFile successfully opened\n");
					}else{
						printf("\tFile does not exist\n");
					}
				}else{
					printf("\tA file is already open\n");
				}
				break;
			case '2': printf("op: %c\n", op);
				break;
			case '3':
				if(fid == NULL){
					printf("\tNo file opened\n");
				}else{
					fclose(fid);
					fid = NULL;
					printf("\tFile closed\n");
				}
				break;
			case '4': printf("op: %c\n", op);
				break;
			case '5': printf("op: %c\n", op);
				break;
			case '6': printf("op: %c\n", op);
				break;
			case '7': printf("op: %c\n", op);
				break;
			default: printf("op: %c\n", op);
				break;
		}
		FLUSH;
		printf("Option: ");
	}
	/*paste(fid, "hello", 5, 2);*/
	/*delete(&fid, 5, 2);*/
	
	/*
	display(fid);
	copy(fid, 6, 0, 6);
	display(fid);*/
	
	/*fseek(fid, -1, SEEK_END);
	c = fgetc(fid);
	printf("%c", c);
	fseek(fid, 0, SEEK_END);
	fputc(c, fid);
	/*printf("Enter Position: ");
	scanf(" %lu", &pos);
	printf("Enter String: ");
	scanf(" %s", str);
	len = strlen(str);
	printf("%zu\n", len);
	
	fseek(fid, , SEEK_END);
	c = fgetc(fid);
	printf("%c\n", c);
	/*c = '6';
	fputc(c, fid);*/
	return 0;
}

/*
Copy:
	prompt user to select starting 'position' and 'length'
	prompt user to enter destination position
	forward shift from destination position to length

cut:
	prompt user to select starting 'position' and 'length'
	prompt user to enter destination position
	use delete after copying to destionation position
*/

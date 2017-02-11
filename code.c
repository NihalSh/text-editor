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
	} else{
		fseek(fid, pos, SEEK_SET);
		pastePos = ftell(fid);
		copyPos = pastePos + (size_t)len;
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
    newfile = NULL;
	newfile = fopen("temp", "r");
	fseek(newfile, 0, SEEK_END);
	endPos = ftell(newfile);
	rewind(newfile);
	/*No other function offers direct deleting, so manual writing to temp file then writing that back to original file*/
	*fid = freopen(NULL, "w+", *fid);
	while (ftell(newfile) < endPos){
		fprintf(*fid, "%c", fgetc(newfile));
	}
	*fid = freopen(NULL, "r+", *fid);
	fclose(newfile);
    newfile = NULL;
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

int copy(FILE* fid, long len, size_t cPos, char ** str){
	int count = 0;
	fseek(fid, cPos, SEEK_SET);
	while(count < len){
		*str = (char *)realloc(*str, sizeof(char)*(count+1));
		(*str)[count] = fgetc(fid);
		count++;
	}
	*str = (char *)realloc(*str, sizeof(char)*(count+1));
	(*str)[count] = '\0';
	return 0;
}

int cut(FILE** fid, long len, size_t cPos, char **str){
    copy(*fid, len, cPos, str);
    delete(fid, len, cPos);
	return 0;
}

int main(){
	int op;
	FILE* fid = NULL;
/*	FILE* fid = fopen("file1", "r+");*/
	char str[50];
    char *clipboard = NULL;
	long int pos;
	size_t len;
	printf("Text Editor\n");
	/*There is no save operation because all the operations are done on the file, that is file is not loaded to memory*/
	printf("Options:\n\t1 New\n\t2 Open\n\t3 Save As\n\t4 Save and Close\n\t5 Cut\n\t6 Copy\n\t7 Paste\n\t8 Delete Text\n\t9 Display Content\n\t0 Quit\n\nOption: ");
	while((op = getchar()) != '0'){
		switch(op){
		case '1':
			if(fid == NULL){
				printf("\tFilename: ");
				scanf(" %s", &str);
				fid = fopen(str, "w+");
				fid = freopen(NULL, "r+", fid);
				if(fid != NULL){
					printf("\tFile successfully created\n");
				}else{
					printf("\tFile does not exist\n");
				}
			}else{
				printf("\tA file is already open\n");
			}
			break;
		case '2':
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
		case '3': printf("op: %c\n", op);
			break;
		case '4':
			if(fid == NULL){
				printf("\tNo file opened\n");
			}else{
				fclose(fid);
				fid = NULL;
				printf("\tFile closed\n");
			}
			break;
		case '5':
            if(fid == NULL){
                printf("\tNo file open\n");
            }else{
                size_t pos;
                long len;
                printf("\tPosition: ");
                scanf(" %zu", &pos);
                printf("\tLength: ");
                scanf(" %zu", &len);
                free(clipboard);/*to avoid memory leak*/
                cut(&fid, len, pos, &clipboard);
                printf("\tText Cut: %s\n", clipboard);
            }
			break;
		case '6':
            if(fid == NULL){
                printf("\tNo file is open\n");
            }else{
                size_t pos;
                long len;
                printf("\tPosition: ");
                scanf(" %zu", &pos);
                printf("\tLength: ");
                scanf(" %zu", &len);
                free(clipboard);/*to avoid memory leak*/
                copy(fid, len, pos, &clipboard);
                printf("\tText Copied: %s\n", clipboard);
            }
			break;
        case '7':
            if(fid == NULL){
                printf("\tNo file is open\n");
            }else if(clipboard == NULL){
                printf("\tClipboard blank\n");
            }else{
                size_t pos;
                printf("\tPosition: ");
                scanf(" %zu", &pos);
                paste(fid, clipboard, strlen(clipboard), pos);
                printf("\tText Pasted\n");
            }
            break;
    	case '8': printf("op: %c\n", op);
			break;
		case '9':
			if(fid != NULL){
				display(fid);
			}else{
				printf("\tNo file opened\n");
			}
			break;
		default: printf("\tNot a valid option!");
			break;
		}
		FLUSH;
		printf("Option: ");
	}
    if(fid != NULL){
        fclose(fid);
        fid = NULL;
    }
    free(clipboard);
    clipboard = NULL;
	return 0;
}


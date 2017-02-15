#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FLUSH while(getchar() != '\n')

int copy(FILE* fid, size_t len, size_t cPos, char ** str);
int cut(FILE** fid, size_t len, size_t cPos, char **str);
int del(FILE** fid, size_t len, size_t pos);
int display(FILE* fid);
int paste(FILE* fid, char* str, size_t len, size_t pos);
int saveas(FILE* fid, char* str);
int shift(FILE* fid, size_t len, size_t pos, char direction);

int main()
{
	int op;
	FILE* fid = NULL;
	char str[50] = {0};
	char *clipboard = NULL;
	printf("Text Editor\n");
	/*There is no save operation because all the operations are done on the file, that is file is not loaded to memory*/
	printf("Options:\n\t1 New\n\t2 Open\n\t3 Save As\n\t4 Save and Close\n\t5 Cut\n\t6 Copy\n\t7 Paste\n\t8 Delete Text\n\t9 Display Content\n\t0 Add Text\n\tq Quit\n\nOption: ");
	while ((op = getchar()) != 'q') {
		switch (op) {
		case '0':
			if (fid != NULL) {
				size_t pos;
				printf("\tPosition: ");
				scanf(" %zu", &pos);
				printf("\tString: ");
				scanf(" %[^\n]", str);
				shift(fid, strlen(str), pos, 0);
				fseek(fid, pos, SEEK_SET);
				fprintf(fid, "%s", str);
				printf("\tText Added\n");
			} else {
				printf("\tNo file is open\n");
			}
			break;
		case '1':
			if (fid == NULL) {
				printf("\tFilename: ");
				scanf(" %[^\n]", &str);
				fid = fopen(str, "w+");
				fid = freopen(NULL, "r+", fid);
				if (fid != NULL) {
					printf("\tFile successfully created\n");
				} else {
					printf("\tFile does not exist\n");
				}
			} else {
				printf("\tA file is already open\n");
			}
			break;
		case '2':
			if (fid == NULL) {
				printf("\tFilename: ");
				scanf(" %[^\n]", &str);
				fid = fopen(str, "r+");
				if (fid != NULL) {
					printf("\tFile successfully opened\n");
				} else {
					printf("\tFile does not exist\n");
				}
			} else {
				printf("\tA file is already open\n");
			}
			break;
		case '3':
			if (fid != NULL) {
				printf("\tNew Filename: ");
				scanf(" %[^\n]", &str);
				saveas(fid, str);
			} else {
				printf("\tNo file open\n");
			}
			break;
		case '4':
			if (fid == NULL) {
				printf("\tNo file opened\n");
			} else {
				fclose(fid);
				fid = NULL;
				printf("\tFile closed\n");
			}
			break;
		case '5':
			if (fid == NULL) {
				printf("\tNo file open\n");
			} else {
				size_t pos;
				size_t len;
				printf("\tPosition: ");
				scanf(" %zu", &pos);
				printf("\tLength: ");
				scanf(" %zu", &len);
				free(clipboard);/*to avoid memory leak*/
				clipboard = NULL;
				cut(&fid, len, pos, &clipboard);
				printf("\tText Cut: %s\n", clipboard);
			}
			break;
		case '6':
			if (fid == NULL) {
				printf("\tNo file is open\n");
			} else {
				size_t pos;
				size_t len;
				printf("\tPosition: ");
				scanf(" %zu", &pos);
				printf("\tLength: ");
				scanf(" %zu", &len);
				free(clipboard);/*to avoid memory leak*/
				clipboard = NULL;
				copy(fid, len, pos, &clipboard);
				printf("\tText Copied: %s\n", clipboard);
			}
			break;
			case '7':
				if (fid == NULL) {
					printf("\tNo file is open\n");
				} else if (clipboard == NULL) {
					printf("\tClipboard blank\n");
				} else {
					size_t pos;
					printf("\tPosition: ");
					scanf(" %zu", &pos);
					paste(fid, clipboard, strlen(clipboard), pos);
					printf("\tText Pasted\n");
				}
				break;
    	case '8':
				if (fid != NULL) {
					size_t len;
					size_t pos;
					printf("\tPosition: ");
					scanf(" %zu", &pos);
					printf("\tLength: ");
					scanf(" %zu", &len);
					del(&fid, len, pos);
					printf("\tText Deleted\n");
				} else {
					printf("\tNo file is open\n");
				}
			break;
		case '9':
			if (fid != NULL) {
				display(fid);
			} else {
				printf("\tNo file opened\n");
			}
			break;
		default: printf("\tNot a valid option!");
			break;
		}
		FLUSH;
		printf("Option: ");
	}
	if (fid != NULL) {
		fclose(fid);
		fid = NULL;
	}
	free(clipboard);
	clipboard = NULL;
	return 0;
}

int copy(FILE* fid, size_t len, size_t cPos, char ** str)
{
	int count = 0;
	fseek(fid, cPos, SEEK_SET);
	while (count < len) {
		*str = (char *)realloc(*str, sizeof(char)*(count+1));
		(*str)[count] = fgetc(fid);
		count++;
	}
	*str = (char *)realloc(*str, sizeof(char)*(count+1));
	(*str)[count] = '\0';
	return 0;
}

int cut(FILE** fid, size_t len, size_t cPos, char **str)
{
	copy(*fid, len, cPos, str);
	del(fid, len, cPos);
	return 0;
}

int del(FILE** fid, size_t len, size_t pos)
{
	char *content = NULL;
	shift(*fid, len, pos, 1);
	fseek(*fid, -1*len, SEEK_END);
	copy(*fid, ftell(*fid), 0, &content);
	*fid = freopen(NULL, "w+", *fid);	
	fprintf(*fid, "%s", content);
	*fid = freopen(NULL, "r+", *fid);
	return 0;
}

int display(FILE* fid)
{
	int c;
	rewind(fid);
	printf("\"");
	while ((c = fgetc(fid)) != EOF) {
		printf("%c", c);
	}
	printf("\"\n");
	rewind(fid);
	return 0;
}

int paste(FILE* fid, char* str, size_t len, size_t pos)
{
	shift(fid, len, pos, 0);
	fseek(fid, pos, SEEK_SET);
	fputs(str, fid);
	return 0;
}

int saveas(FILE* fid, char* str)
{
    int c;
    FILE* newfile = fopen(str, "w+");
    if (newfile != NULL) {
        while ((c = fgetc(fid)) != EOF) {
            fprintf(newfile, "%c", c);
        }
        fclose(newfile);
        return 0;
    } else {
        return 1;
    }
}

int shift(FILE* fid, size_t len, size_t pos, char direction)
{
	size_t copyPos;
	size_t pastePos;
	int c;
	if (direction == 0) {
		fseek(fid, -1, SEEK_END);
		copyPos = ftell(fid);
		pastePos = copyPos + len;
	} else {
		fseek(fid, pos, SEEK_SET);
		pastePos = ftell(fid);
		copyPos = pastePos + len;
	}

	while ((copyPos || direction) && (copyPos >= pos)) {
		fseek(fid, copyPos, SEEK_SET);
		c = fgetc(fid);
		fseek(fid, pastePos, SEEK_SET);
		fputc(c, fid);
		if (direction == 0) {
			copyPos--;
			pastePos--;
		} else {
			fseek(fid, 0, SEEK_END);
			if (ftell(fid) - 1 > copyPos) {
				copyPos++;
				pastePos++;
			} else {
				break;
			}
		}
	}
	return 0;
}

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#define MAX_BUF 255

void listFiles(char *currentDir);
void findFile(char *fileName, char *startingDir);
void grepFile(char *phrase, char *fileName);
bool found = false;

int main (void) {

	char input[200];
	char *args[10];
	char *tok;
	int index = 0;
	char mycwd[MAX_BUF];

	if (getcwd(mycwd, MAX_BUF) == NULL) {	/*grab current directory into "mycwd"*/
		perror("Failed to get current working directory");
		exit(1);
	}

	while(true) {

		printf("Enter command or type 'end' to quit-> ");
		fgets(input, 200, stdin);	/*gather input*/
		input[strlen(input) - 1] = '\0';

		tok = strtok(input, " ");	/*put input into tokenized array 'args'*/
		while (tok != NULL) {
			args[index] = tok;
			tok = strtok(NULL, " ");	
			index++;
		}
		args[index] = NULL;

		if (args[0] == NULL) {

			printf("No arguments provided\n");

		} else if (strcmp(args[0], "end") == 0) {	/*end session*/

			printf("ending session...\n");
			exit(0);

		} else if ((strcmp(args[0], "ls") == 0) && (strcmp(args[1], "-l") == 0)) {

			listFiles(mycwd);	/*long-list function*/

		} else if ((strcmp(args[0], "find") == 0) && (args[1] != NULL) && (args[2] != NULL)) {

			findFile(args[1], args[2]);	/* call find function*/
			chdir(mycwd);	/*reset to original directory*/

			if (!found) {
				printf("couldn't find file, try again\n");
			}

		} else if ((strcmp(args[0], "grep") == 0) && (args[1] != NULL) && (args[2] != NULL)) {

			grepFile(args[1], args[2]); /*execute grep function*/

		} else {
			printf("unknown command\n");
		}

		index = 0;
		found = false;
	}

	return 0;

}

void listFiles(char *currentDir) {
	struct dirent *direntp;
	struct stat statbuf;
	DIR *dirp;

	if ((dirp = opendir(currentDir)) == NULL) {
		perror("Failed to open current directory");
		//exit(1);
	}

	while ((direntp = readdir(dirp)) != NULL) {

		if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0)) {
			continue;
		} else {
			if (stat(direntp->d_name, &statbuf) == -1) {
				perror("failed to get file status");
				//exit(1);
			}

			printf("File Name: %s\n", direntp->d_name);

			printf("Permissions: ");
			printf( (S_ISDIR(statbuf.st_mode)) ? "d" : "-");
			printf( (statbuf.st_mode & S_IRUSR) ? "r" : "-");
			printf( (statbuf.st_mode & S_IWUSR) ? "w" : "-");
			printf( (statbuf.st_mode & S_IXUSR) ? "x" : "-");
			printf( (statbuf.st_mode & S_IRGRP) ? "r" : "-");
			printf( (statbuf.st_mode & S_IWGRP) ? "w" : "-");
			printf( (statbuf.st_mode & S_IXGRP) ? "x" : "-");
			printf( (statbuf.st_mode & S_IROTH) ? "r" : "-");
			printf( (statbuf.st_mode & S_IWOTH) ? "w" : "-");
			printf( (statbuf.st_mode & S_IXOTH) ? "x\n" : "-\n");

			printf("Owner: %ld\n", (long)statbuf.st_uid);

			printf("Group: %ld\n", (long)statbuf.st_gid);

			printf("Bytes: %ld\n", (long)statbuf.st_size);

			printf("Date last modified: %s\n", ctime(&statbuf.st_mtime));
		}
	}

	/*try to close directory until it happens*/
	while ((closedir(dirp) == -1) && (errno == EINTR))  ;
}

void findFile(char *fileName, char *startingDir) {
	struct dirent *direntp;
	struct stat statbuf;
	DIR *dirp;
	char absoluteDir[MAX_BUF];

	chdir(startingDir);

	if ((dirp = opendir(startingDir)) == NULL) {
		//fprintf(stderr, "cannot open directory %s: %s\n", startingDir, strerror (errno));
		//exit(1);
	} else {

		while ((direntp = readdir(dirp)) != NULL) {

			if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0)) {
				continue;
			} else {
				char currentDir[MAX_BUF];
				strcpy(currentDir, startingDir);
				strcat(currentDir, "/");
				strcat(currentDir, direntp->d_name);

				if (stat(currentDir, &statbuf) == -1) {
				//fprintf(stderr, "cannot get file status on %s: %s\n", currentDir, strerror (errno));
				//exit(1);
				} 
		
				if (strcmp(direntp->d_name, fileName) == 0) {
			
					found = true;
					char buf[MAX_BUF];
					realpath(direntp->d_name, buf);
					printf("%s\n", buf);

				} else if (S_ISDIR(statbuf.st_mode)) {	//if directory, recursively call findFile
					char buf[MAX_BUF];
					strcpy(buf, startingDir);
					strcat(buf, "/");
					strcat(buf, direntp->d_name);

					findFile(fileName, buf);

				}
			}
		}
		/*close open directories*/
		while ((closedir(dirp) == -1) && (errno == EINTR))  ;
	}
	
}

void grepFile(char *phrase, char *fileName) {
	FILE *file;
	char line[100];
	char temp[100];
	int lineNum = 0;
	char *token;
	char *tokenHolder[100];
	int tokenNum = 0;
	int i;

	file = fopen(fileName, "r");

	if (file == NULL) {
		fprintf(stderr, "cannot open file for reading %s: %s\n", fileName, strerror (errno));
	} else {

		while (fgets(line, sizeof(line), file)) { /*gather 1 line of 100 chars*/
	
			strcpy(temp, line); /*copy line to temp b/c tokenizing destroys line*/

			token = strtok(line, " ");	/*put words of line into tokenized array 'tokenHolder'*/
			while (token != NULL) {
				tokenHolder[tokenNum] = token;
				token = strtok(NULL, " ");	
				tokenNum++;
			}
			tokenHolder[tokenNum] = NULL;

			for (i=0; i<tokenNum; i++) {
				if (strcmp(tokenHolder[i], phrase) == 0) {  /*find match in tokenized line*/
					printf("Line # %d\n%s\n", lineNum, temp);
					break;
				}
			}

			lineNum++;
			tokenNum = 0;
		}
		fclose(file);
	}	
}



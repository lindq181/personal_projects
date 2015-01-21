#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include "fifoHeader.h"
#define FIFOARG 1
#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH | O_NONBLOCK)
#define BLKSIZE PIPE_BUF

//declare threadListener
void *threadListener();
int readline(int fd, char *buf, int nbytes);
ssize_t r_write(int fd, void *buf, size_t size);

//global storage for phone#-name pair
const char *storage[6] = {
"6122058229", "Jon",
"1234567890", "Jane",
"0987654321", "Jake",
};

char buf[PIPE_BUF];
int ret;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int wellKnownFd;

int main (int argc, char *argv[]) {
	int len;
	pthread_t worker[3];
	ret = pthread_mutex_init(&mutex, NULL);

	if ((mkfifo(wellKnownFifo, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("Server failed to create a FIFO");
		return 1;
	}

	if ((wellKnownFd = open(wellKnownFifo, O_RDWR)) == -1) {
		perror("Server failed to open its FIFO");	
		return 1;
	}

	//make 3 worker threads server-side, they start at threadListener

	pthread_create(&worker[0], NULL, threadListener, NULL);
	pthread_create(&worker[1], NULL, threadListener, NULL);
	pthread_create(&worker[2], NULL, threadListener, NULL);


	if (pthread_join(worker[0], NULL)) {
		perror("Error joining thread\n");
		exit(1);
	}
	if (pthread_join(worker[1], NULL)) {
		perror("Error joining thread\n");
		exit(1);
	}
	if (pthread_join(worker[2], NULL)) {
		perror("Error joining thread\n");
		exit(1);
	}
	pthread_exit(NULL);
	
	return 1;
}

void *threadListener() {
	while (true) {
		/*lock to read from client*/
		ret = pthread_mutex_lock(&mutex);
		readline(wellKnownFd, buf, PIPE_BUF);
		pthread_t current = pthread_self();
		printf("Inside %d thread, %s\n", (int)current, buf);
		ret = pthread_mutex_unlock(&mutex);
		/*unlock*/

		/*locally used variables within thread*/
		char clientstring[PIPE_BUF];
		int clientpid;
		char phonenumber[9];
		int found = false;
		char name[PIPE_BUF];
		int i, k, j;
			
		/*grab the clientpid into string*/
		memcpy(clientstring, buf, 4);
		printf("clientstring \"%s\"\n", clientstring);
		clientpid = atoi(clientstring);

		/*get phone number*/
		memcpy(phonenumber, buf + 4, 10);

		for (i = 0; i<6; i=i+2) {
			if (strcmp(storage[i], phonenumber) == 0) {
				found = true;
				memcpy(name, storage[i+1], 10);
			}
		}

		if (found == true) {
			printf("found number: %s\n", name);
		}

		int clientfd;
		/*open client specific fifo for response, name is client id as string*/
		if ((clientfd = open(clientstring, O_WRONLY)) == -1) {
			perror("Server failed to open its FIFO for client response");	
			exit(1);
		}

		int returnlen = strlen(name);
		if (r_write(clientfd, name, returnlen) != returnlen) {
			perror("Server failed to write to client");
			exit(1);
		}

		r_close(clientfd);
	}
	
}
	

int readline(int fd, char *buf, int nbytes){

	int numread = 0;
	int returnval;

	while (numread < nbytes - 1) {
		returnval = read(fd, buf + numread, 1);
		if ((returnval == -1) && (errno != EINTR)) {
			continue;
		}
		if ((returnval == 0) && (numread == 0)) {
			return 0;
		}
		if (returnval == 0) {
			break;
		}
		if (returnval == -1) {
			return -1;
		}
		numread++;
		if (buf[numread-1] == '\n') {
			buf[numread] == '\0';
			return numread;
		}
	}
	errno = EINVAL;
	return -1;
}

ssize_t r_write(int fd, void *buf, size_t size){
	char *bufp;
	size_t bytestowrite;
	ssize_t byteswritten;
	size_t totalbytes;

	for (bufp = buf, bytestowrite = size, totalbytes = 0;
	     bytestowrite > 0;
	     bufp += byteswritten, bytestowrite -= byteswritten) {
		byteswritten = write(fd, bufp, bytestowrite);
		if ((byteswritten) == -1 && (errno != EINTR)) {
			return -1;
		}
		if (byteswritten == -1) {
			byteswritten = 0;
		}
		totalbytes += byteswritten;
	}
	return totalbytes;
}

int r_close(int fildes) {
	int retval;
	while (retval = close(fildes) && retval == -1 && errno == EINTR) ;
	return retval;
}



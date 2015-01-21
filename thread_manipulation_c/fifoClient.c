#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include "fifoHeader.h"
#define FIFOARG 1
#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH | O_NONBLOCK)

const char *storage[3] = {
"6122058229",
"1234567890",
"0987654321",
};

int r_close(int fildes);
ssize_t r_write(int fd, void *buf, size_t size);
int readline(int fd, char *buf, int nbytes);

int main (int argc, char *argv[]) {
	int len;
	char requestbuf[PIPE_BUF];
	int wellKnownFd;
	int clientfd;
	char clientstring[PIPE_BUF];
	char retval[PIPE_BUF];

	sprintf(clientstring, "%ld", (long)getpid());

	/*open well known fifo pipe*/
	if ((wellKnownFd = open(wellKnownFifo, O_WRONLY)) == -1) {
		perror("Client failed to open log fifo for writing");
		return 1;
	}

	if ((mkfifo(clientstring, FIFO_PERMS) == -1) && (errno != EEXIST)) {
		perror("client failed to create FIFO for server response");
		return 1;
	}

	/*randomly grab a number out of the storage array*/
	srand(time(NULL));
	int decision = rand() % 3;

	/*build request buffer*/
	snprintf(requestbuf, PIPE_BUF, "%ld%s\n", (long)getpid(), storage[decision]);
	len = strlen(requestbuf);

	/*write the random request to server*/
	printf("Chosen #: %s, Client ID: %s\n", storage[decision], clientstring);
	if (r_write(wellKnownFd, requestbuf, len) != len) {
		perror("Client failed to write");
		return 1;
	}
	/*close well-known*/
	r_close(wellKnownFd);

	/*open client specific fifo*/
	if ((clientfd = open(clientstring, O_RDONLY)) == -1) {
		perror("client failed to open its FIFO for client response");	
		return 1;
	}

	readline(clientfd, retval, PIPE_BUF);
	printf("%s\n", retval);
	unlink(clientstring);
	return 0;
}

int r_close(int fildes) {
	int retval;
	while (retval = close(fildes) && retval == -1 && errno == EINTR) ;
	return retval;
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

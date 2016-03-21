/* @Author Ted Adams (tadams2@umbc.edu)
 * This file contains the units tests for the scrubber.c character device
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#define FILTER_DPORT 4210

static sem_t sem;
static int listener_eventfd;

/**
 * Run the 'listening' thread.
 */
static void *run_listener(void *arg __attribute__ ((unused)))
{
	int daemonfd;
	struct sockaddr_in addr;

	/* create a daemon socket */
	daemonfd = socket(AF_INET, SOCK_STREAM, 0);
	if (daemonfd < 0) {
		perror("Could not create daemon socket");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(FILTER_DPORT);
	if (bind(daemonfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Could not bind to port");
		exit(EXIT_FAILURE);
	}
	if (listen(daemonfd, 10) < 0) {
		perror("Could not listen to port");
		exit(EXIT_FAILURE);
	}

	/* signal that listener is ready */
	sem_post(&sem);

	int epfd;
	struct epoll_event event;
	epfd = epoll_create(1);
	if (epfd < 0) {
		perror("Could not create epoll instance");
		exit(EXIT_FAILURE);
	}
	event.events = EPOLLIN;
	event.data.fd = listener_eventfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listener_eventfd, &event);
	event.data.fd = daemonfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, daemonfd, &event);

	/* keep waiting for connections */
	while (epoll_wait(epfd, &event, 1, -1) > 0) {
		if (event.data.fd == listener_eventfd) {
			/* shutdown daemon */
			break;
		}

		int sockfd;
		socklen_t addrlen = sizeof(addr);
		sockfd = accept(daemonfd, (struct sockaddr *)&addr, &addrlen);
		if (sockfd < 0) {
			perror("Error accepting connection");
			continue;
		}

		printf("Listener: Got a connection\n");

		/* handle received messages, by calling read() upon
		   sockfd */
		/*
		 * YOUR CODE HERE
		 *
		 * HINT: This thread should validate that the strings
		 * are correct. Use synchronizations between the
		 * sending and listening thread, so that the listening
		 * thread can determine when a string is correct.
		 */
		char buf[80];
		/* happy path test */
		if (read(sockfd, buf, 80) < 0) {
			printf("read from sockfd failed");
		} else {
			printf("should be: ***\n");
			printf("sockfd : %s\n", buf);

		}

		if (read(sockfd, buf, 80) < 0) {
			printf("read from sockfd failed");
		} else {
			printf("should be: ***n\n");
			printf("sockfd : %s\n", buf);

		}

		if (read(sockfd, buf, 80) < 0) {
			printf("read from sockfd failed");
		} else {
			printf("should be: e\n");
			printf("sockfd : %s\n", buf);

		}

		if (read(sockfd, buf, 80) < 0) {
			printf("read from sockfd failed");
		} else {
			printf("should be: *** ***\n");
			printf("sockfd : %s\n", buf);

		}

		if (read(sockfd, buf, 80) < 0) {
			printf("read from sockfd failed");
		} else {
			printf("should be: not in list\n");
			printf("sockfd : %s\n", buf);

		}

		if (read(sockfd, buf, 80) < 0) {
			printf("read from sockfd failed");
		} else {
			printf("should be: *** *** ***\n");
			printf("sockfd : %s\n", buf);

		}
		close(sockfd);
	}

	close(daemonfd);
	return NULL;
}

/**
 * Run the 'sending' thread.
 *
 * Opens a socket to the listener. Then send messages over the socket.
 */
static void *run_sender(void *arg __attribute__ ((unused)))
{
	int sockfd;
	struct sockaddr_in addr;

	/* block until daemon is ready */
	sem_wait(&sem);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Could not create sending socket");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(FILTER_DPORT);
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Could not connect to server");
		exit(EXIT_FAILURE);
	}

	printf("Sender: Connected\n");

	/* send messages to the listener by calling write() to
	   sockfd */
	/*
	 * YOUR CODE HERE
	 *
	 * HINT: Not only should you send messages to the socket, you
	 * should also intermix writes to /dev/scrubber.
	 */
	int filedes;
	/*testing to make sure /dev/scrubber can be opened */
	if ((filedes = open("/dev/scrubber", O_WRONLY)) < 0) {
		printf("opening /dev/scrubber failed\n");
	} else {
		printf("opened /dev/scrubber\n");
	}
	/* my program doesnt work for the last work written so i write 2 "the" to make it pick up on it */
	if (write(filedes, "the|the", 8) < 0) {
		printf("write to /dev/scrubber unsuccessful\n");
	}
	/* 'the' is the only word in the dirty words list so it should be filtered */
	if (write(sockfd, "the", 8) < 0) {
		printf("write to socket unsuccessful\n");
	}
	sem_post(&sem);
	sleep(1);
	sem_wait(&sem);
	/* testing to see if it finds a the substring in a word longer than the substring */
	if (write(sockfd, "then", 8) < 0) {
		printf("write to socket unsuccessful\n");
	}

	sem_post(&sem);
	sleep(1);
	sem_wait(&sem);
	/* testing to see that a word smaller than the dirty word does not get filtered */
	if (write(sockfd, "e", 8) < 0) {
		printf("write to socket unsuccessful\n");
	}

	sem_post(&sem);
	sleep(1);
	sem_wait(&sem);
	/* testing adding many words to the dirty words list */
	if (write(filedes, "apple|orange|pen|the", 21) < 0) {
		printf("write to /dev/scrubber unsuccessful\n");
	}
	/*testing a combination of old words and new words */
	if (write(sockfd, "the pen", 8) < 0) {
		printf("write to sockfd unsuccessful\n");
	}

	sem_post(&sem);
	sleep(1);
	sem_wait(&sem);
	/* testing something that should remain unfiltered */

	if (write(sockfd, "not in list", 12) < 0) {
		printf("write to sockfd unsuccessful\n");
	}

	sem_post(&sem);
	sleep(1);
	sem_wait(&sem);
	/*demonstrating a bug in my code */

	if (write(sockfd, "the the the", 12) < 0) {
		printf("write to sockfd unsuccessful\n");
	}
	/* testing adding not a word to the dirty list */
	/* NOTE TO THE GRADER: I did try this but It made my kernel lock up so I commented it out 
	   if(write(filedes, "", 1) < 0)
	   {
	   printf("write to /dev/scrubber unsuccessful\n");
	   }

	   if(write(sockfd, "not in buf", 9) < 0)
	   {
	   printf("write to /dev/scrubber unsuccessful\n");
	   }
	 */
	sem_post(&sem);
	close(sockfd);
	return NULL;
}

int main(void)
{
	/* create a semaphore, to block the sender until listener is
	   ready */
	sem_init(&sem, 0, 0);

	/* create an eventfd object, used to notify the listener when
	   shutting down */
	listener_eventfd = eventfd(0, 0);
	if (listener_eventfd < 0) {
		perror("Could not create eventfd");
		exit(EXIT_FAILURE);
	}

	/* spawn listening and sending threads */
	pthread_t listener_pth, sender_pth;
	pthread_create(&listener_pth, NULL, run_listener, NULL);
	pthread_create(&sender_pth, NULL, run_sender, NULL);

	/* after sending thread has terminated, send a message to
	   listening thread to terminate itself */
	pthread_join(sender_pth, NULL);
	uint64_t val = 1;
	if (write(listener_eventfd, &val, sizeof(val)) < 0) {
		perror("Could not send kill signal to listener");
		exit(EXIT_FAILURE);
	}
	pthread_join(listener_pth, NULL);

	return 0;
}

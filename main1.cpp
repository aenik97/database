#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cctype>
#include <time.h>
#include "headers.h"
#define PORT 5555
int read_from_socket(int, char *, int);
int write_to_socket(int, const char *);
int my_printf(int fd, const char *format, ...) {
	char buf[LEN]; int len;
	va_list ap;
	va_start(ap, format);
	len = vsnprintf(buf, LEN, format, ap);
	if (len <= 0) 
		return len;
	len = write_to_socket(fd, buf);
	va_end(ap);
	return len;
}
int read_from_socket(int fd, char *buf, int maxlen) {
	int len, i, nbytes;
	if (read(fd, &len, sizeof(int)) != (int)sizeof(int))
		return -1;
	if (len > maxlen) 
		return -2;
	for (i = 0; len > 0; i += nbytes, len -= nbytes) {
		nbytes = read(fd, buf + i, len);
		if (nbytes < 0) 
			return -3;
		else 
			if (nbytes == 0) 
				return -4;
	}
	return i;
}
int write_to_socket(int fd, const char *buf) {
	int len, i, nbytes;
	len = strlen(buf) + 1;
	if (write(fd, &len, sizeof(int)) != (int)sizeof(int)) 
		return -1;
	for (i = 0; len > 0; i += nbytes, len -= nbytes) {
		nbytes = write(fd, buf + i, len);
		if (nbytes < 0) 
			return -2;
		else 
			if (nbytes == 0) 
				return -3;
	}
	return i;
}
int main(int argc, char **argv) {
	if (argc < 1) 
		printf("Use: %s filename.txt\n", argv[0]);
	char buf[LEN];
	double time = clock(), cclock = 0;
	int sock, opt = 1, i, count = 0, n = 0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "Cannot open DB!\n");
	 	return -1;
	}
	command * currentCommand = new command;
	dataBase *db = new dataBase(100);
	printf("Loading...\n");
	printf("READ AND CONSTRUCTION OF STRUCTURES \n");
	if (db -> build(fp) == -3) { 
		delete db;
		delete currentCommand;
		fclose(fp);
		return -1;
	}
	else 
		printf("SERVER IS READY!\n");
	time = clock() - time;
	cclock += time;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		fprintf(stderr, "Can't open socket!\n");
		return -2;
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		fprintf(stderr, "Can't bind socket!\n");
		return -3;
	}
	if (listen(sock, 3)) 
		return 3;
	fd_set active_set, read_set;
	FD_ZERO(&active_set);
	FD_SET(sock, &active_set);    	
	for (;;) {
		read_set = active_set;
		if (select(FD_SETSIZE, &read_set, 0, 0, 0) < 0) 
			return 4;
		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_set)) {
				if (i == sock) {
					unsigned size = sizeof(client);
					int new_sock = accept(sock, (struct sockaddr *)&client, &size);
					if (new_sock < 0) {
						fprintf(stderr, "Can't accept!\n");
						fclose(fp);
						delete db;
						delete currentCommand;
						time = clock() - time;
						cclock += time;
					    printf ("||WORK TIME: %fs||\n", cclock / CLOCKS_PER_SEC);
						close(sock);
						return -1;
					}
					fprintf(stderr, "Client %d connected\n", count++);
					inet_ntoa(client.sin_addr);
					FD_SET(new_sock, &active_set);
				}
				else {
					int len = read_from_socket(i, buf, LEN);
					for (int j = 0; buf[j]; j++) {
						if (buf[j] == '\n')	{
							buf[j] = 0;
							break;
						}
					}
					printf("%s\n", buf);
					currentCommand->init(buf);
					if (len < 0) {
						close(i);
						currentCommand->~command();
						FD_CLR(i, &active_set);
					}
					else {
						if ((buf[0] == 's')&&(buf[1] == 't')) {
							close(sock);
							close(i);
							FD_CLR(i, &active_set);
							write_to_socket(i, "END");
							fclose(fp);
							currentCommand->~command();
							delete db;
							delete currentCommand;
							time = clock() - time;
							cclock += time;
						    	printf ("||WORK TIME: %fs||\n", cclock / CLOCKS_PER_SEC);
							return 0;
						}
						else 
							if (buf[0] == 'q') {
								currentCommand->~command();
								close(i);
								FD_CLR(i, &active_set);	
								write_to_socket(i, "END");
						}
						else {
							my_printf(i, "---------------------------------------------------------\n");
							my_printf(i, "%s\n", buf);
							currentCommand->init(buf);
							if (db -> apply(stdout, currentCommand) == -2) {
								my_printf(i, "Unknown command\n");
								write_to_socket(i, "ERROR");
								close(i);
								close(sock);
								FD_CLR(i, &active_set);	
								fclose(fp);
								currentCommand->~command();
								delete db;
								delete currentCommand;
								time = clock() - time;
								cclock += time;
							    	printf ("||WORK TIME: %fs||\n", cclock / CLOCKS_PER_SEC);
								return 0;
							}
							else {
								write_to_socket(i, "DONE");
								currentCommand->~command();
								my_printf(i, "\n");
							}
						}
					}
				}
			}
		}
		i = FD_SETSIZE;
	}
	fclose(fp);
	delete db;
	delete currentCommand;
	close(sock);
	time = clock() - time;
	cclock += time;
    	printf ("||WORK TIME: %fs||\n", cclock / CLOCKS_PER_SEC);
  	return 0;
}

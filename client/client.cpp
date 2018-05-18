#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 5555
#define LEN 1234
int read_from_socket(int, char *, int);
int write_to_socket(int, const char *);
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
int main() {
	int sock;
	struct sockaddr_in server_addr;
	struct hostent *hostinfo;
	char buf[LEN];
	hostinfo = gethostbyname(SERVER_NAME);
	if (!hostinfo) {
		fprintf(stderr, "Can't get host!\n");
		return -1;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr = *(struct in_addr *)hostinfo -> h_addr;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		fprintf(stderr, "Can't open socket!\n");
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		fprintf(stderr, "Can't connect!\n");
		return -1;
	}
	while (fgets(buf, LEN, stdin)) {
		for (int i = 0; buf[i]; i++) {
			if (buf[i] == '\n') {
				buf[i] = 0;
				break;
			}
		}
		if (!strcmp(buf, "quit;") || !strcmp(buf, "stop;")) {
			write_to_socket(sock, buf);
			read_from_socket(sock, buf, LEN);
			close(sock);
			return 0;
		}
		if (write_to_socket(sock, buf) < 0) 
			return -1;
		for (;;) {
			read_from_socket(sock, buf, LEN);
			if (!strcmp(buf, "DONE"))
				break;
			if (!strcmp(buf, "ERROR")) {
				close(sock);
				return -1;
			}
			printf("%s", buf);
		}
	}
	close(sock);
	return 0;
}

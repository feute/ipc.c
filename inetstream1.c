#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DATA "Half a league, half a league ..."

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line. One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is: 
 * inetstream1 <hostname> <port>
 */

int
main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;

	if (argc < 3) {
		fprintf(stderr, "usage: %s <hostname> <port>\n", argv[0]);
		exit(1);
	}

	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}

	/* connect socket using name specified by command line */
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		fprintf(stderr, "%s: unknown host.", argv[1]);
		exit(2);
	}
	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
	server.sin_port = htons(strtol(argv[2], NULL, 10));

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connecting stream socket");
		exit(1);
	}

	if (write(sock, DATA, sizeof(DATA)) < 0)
		perror("writing on stream socket");

	close(sock);
	return 0;
}

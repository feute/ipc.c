#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define DATA "Half a league, half a league ..."

/*
 * This program connects to the socket name in the command line and sends a
 * one line message to that socket. The form of the command line is:
 * unixstream1 <pathname>
 */

int
main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_un server;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <pathname>\n", argv[0]);
		exit(1);
	}

	/* create socket */
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}

	/* connect socket using name specified by command line */
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, argv[1]);

	if (connect(sock, (struct sockaddr *)&server, 
			sizeof(struct sockaddr_un)) < 0) {
		close(sock);
		perror("connecting stream socket");
		exit(1);
	}

	if (write(sock, DATA, sizeof(DATA)) < 0)
		perror("writing on stream socket");

	return 0;
}

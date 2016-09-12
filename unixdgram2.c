#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

#define DATA "The sea is calm tonight, the tide is full ..."

/*
 * Here I send a datagram to a receiver whose name I get from the command
 * line arguments. The form of the command line is unixdgram2 <pathname>
 */

int
main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_un name;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <socket file>\n", argv[0]);
		exit(1);
	}

	/* create socket on which to send */
	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("opening datagram socket");
		exit(1);
	}

	/* construct name of socket to send to */
	name.sun_family = AF_UNIX;
	strcpy(name.sun_path, argv[1]);

	/* send message */
	if (sendto(sock, DATA, sizeof(DATA), 0, 
		(struct sockaddr *)&name, sizeof(name)) < 0 ) {
		perror("sending datagram message");
	}

	close(sock);
	return 0;
}

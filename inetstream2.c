#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */

int
main(void)
{
	int sock, msgsock, rval, i;
	socklen_t length;
	char buf[1024];
	struct sockaddr_in server;

	/* Create socket. */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}

	/* Name socket using wildcards. */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 0;
	if (bind(sock, &server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}

	/* Find out assigned port number and print it out. */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length)) {
		perror("getting sock name");
		exit(1);
	}
	printf("Socket has port: %d.\n", ntohs(server.sin_port));

	/* Start accepting connections. */
	listen(sock, 5);
	do {
		msgsock = accept(sock, 0, 0);
		
		if (msgsock == -1) {
			perror("accept");
		} else {
			do {
				bzero(buf, sizeof(buf));

				if ((rval = read(msgsock, buf, 1024)) < 0)
					perror("reading stream message");

				if (rval == 0)
					printf("Ending connection.\n");
				else
					printf("-->%s\n", buf);
			} while (rval != 0);
		}

		close(msgsock);
	} while (1);
	/*
	 * Since this program has an infinite loop, the socket "sock" is
	 * never explicitly closed. However, all sockets will be closed
	 * automatically when a process is killed or terminates normally.
	 */

	return 0;
}

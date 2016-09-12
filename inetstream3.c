#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 * This program uses select(2) to check that someone is trying to connect
 * before calling accept(2).
 */

int
main(void)
{
	int sock, msgsock, rval;
	socklen_t length;
	char buf[1024];
	fd_set ready;
	struct sockaddr_in server;
	struct timeval to;

	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}

	/* name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 0;

	if (bind(sock, (struct sockaddr *)&server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}

	/* finding out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length)) {
		perror("getting socket name");
		exit(1);
	}
	printf("socket has port: %d\n", ntohs(server.sin_port));

	/* start accepting connections */
	listen(sock, 5);
	do {
		FD_ZERO(&ready);
		FD_SET(sock, &ready);
		to.tv_sec = 5;

		if (select(sock + 1, &ready, 0, 0, &to) < 0) {
			perror("select");
			continue;
		}

		if (FD_ISSET(sock, &ready)) {
			msgsock = accept(sock, (struct sockaddr *)0, (socklen_t *)0);
			if (msgsock == -1) {
				perror("accept");
			} else {
				do {
					bzero(buf, sizeof(buf));

					if ((rval = read(msgsock, buf, 1024)) < 0)
						perror("reading stream message");
					else if (rval == 0)
						printf("ending connection\n");
					else
						printf("-->%s\n", buf);
				} while (rval > 0);
			}
		} else {
			printf("do something else\n");
		}
	} while (1);

	return 0;
}


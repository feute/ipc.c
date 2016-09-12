#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define NAME "socket"

/*
 * This program creates a socket in the UNIX domain and binds a name to it.
 * After printing the socket's name, it begins a loop. Each time through the
 * loop it accepts a connection and prints out messages from it. When the
 * connection breaks, or a termination message comes through, the program
 * accepts a new connection.
 */

int
main(void)
{
	int sock, msgsock, rval;
	char buf[1024];
	struct sockaddr_un server;

	/* create socket */
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}

	/* name socket using filesystem name */
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, NAME);
	if (bind(sock, (struct sockaddr *)&server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}
	printf("socket has name %s\n", server.sun_path);

	/* start accepting connections */
	listen(sock, 5);
	while (1) {
		msgsock = accept(sock, 0, 0);
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
		close(msgsock);
	}

	/*
	 * The following statements are not executed, because they follow an
	 * infinite loop. However, most ordinary programs will not run forever.
	 * In the UNIX domain it is necessary to tell the filesystem that one is
	 * through using NAME. In most programs one uses the call unlink(2) as
	 * below. Since the user will have to kill this program, it will be
	 * necessary to remove the name by a command from the shell.
	 */
	close(sock);
	unlink(NAME);
	return 0;
}

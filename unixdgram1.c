#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

/*
 * In the included file <sys/un.h> a sockaddr_un is defined as follows:
 * struct sockaddr_un {
 *		short sun_family;
 *		char  sun_path[108];
 * };
 */

#define NAME "socket"

/*
 * This program creates a UNIX domain datagram socket, binds a name to it,
 * then reads from the socket.
 */

int
main(void)
{
	int sock;
	char buf[1024];
	struct sockaddr_un name;

	/* create socket from which to read */
	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("opening datagram socket");
	}

	/* create name */
	name.sun_family = AF_UNIX;
	strcpy(name.sun_path, NAME);
	if (bind(sock, (struct sockaddr *)&name, sizeof(name))) {
		perror("binding name to datagram socket");
		exit(1);
	}
	
	printf("socket file: %s\n", NAME);
	/* Read from the socket. */
	if (read(sock, buf, 1024) < 0)
		perror("receiving datagram packet");

	printf("-->%s\n", buf);

	close(sock);
	unlink(NAME);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils_v2.h"
#include "network.h"

#define MAX_ZOMBIES 3

const int tabPorts[NBR_PORTS] = {
	SERVER_PORT1,
	SERVER_PORT2,
	SERVER_PORT3,
	SERVER_PORT4,
	SERVER_PORT5,
	SERVER_PORT6,
	SERVER_PORT3,
	SERVER_PORT4,
	SERVER_PORT5,
	SERVER_PORT6};

int[] testPorts(char *ip)
{
	int sockfdTab[];
	int indexTab = 0;

	for (size_t i = 0; i < NBR_PORTS; i++)
	{
		int sockfd = ssocket();
		int res = sconnect(serverIP, serverPort, sockfd);
		if (res == 0){
			sockfdTab[indexTab] = sockfd;
			indexTab++;
		}
	}
// TODO
	return sockfdTab;
}

int initSocketServer(int port)
{
	int sockfd = ssocket();

	/* no socket error */

	// setsockopt -> to avoid Address Already in Use
	// to do before bind !
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

	sbind(port, sockfd);

	/* no bind error */
	slisten(sockfd, MAX_ZOMBIES);

	/* no listen error */
	return sockfd;
}

int disconnectZombies()
{
	return 0;
	// TODO
}
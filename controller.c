#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#include "utils_v2.h"
#include "network.h"

int nbrSocketsConnected;
int sockFdPortsConnectedTab[30] = {0};

void sendCommandToZombies()
{
    char command[256];
    while (fgets(command, 256, stdin) != NULL)
    {
        for (size_t i = 0; i < nbrSocketsConnected; i++)
        {
            swrite(sockFdPortsConnectedTab[i], command, strlen(command));
        }
    }
}

void listenToResponse()
{
    struct pollfd fds[30];
    char buffer[2000];
    int nbZombisConnected = nbrSocketsConnected;

    for (int i = 0; i < nbrSocketsConnected; i++)
    {
        // init poll
        fds[i].fd = sockFdPortsConnectedTab[i];
        fds[i].events = POLLIN;
    }

    while (nbZombisConnected>0)
    {
        spoll(fds, nbrSocketsConnected, 10);
        for (int i = 0; i < nbrSocketsConnected; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int nbChar = sread(fds[i].fd, buffer, 2000);

                if (nbChar == 0)
                {
                    nbZombisConnected--;
                    printf("\nPlus de zombies à écouter\n");
                    skill(getppid(),SIGINT);
                    exit(0);
                }

                swrite(1, buffer, nbChar);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Pas d'ip en argument\n");
        exit(1);
    }

    for (size_t i = 1; i < argc; i++)
    {
        const char *ip = argv[i];
        nbrSocketsConnected = testAndConnectPorts(ip, sockFdPortsConnectedTab);
    }

    printf("Entrez des commandes à envoyer aux zombies et les réponses s'afficheront ici\n");

    pid_t pid = sfork();

    if (pid == 0)
    {
        // FILS
        listenToResponse();
    }
    else
    {
        // PERE
        sendCommandToZombies();
        disconnectZombies(nbrSocketsConnected, sockFdPortsConnectedTab);
        skill(pid,SIGINT);
        exit(0);
    }
}

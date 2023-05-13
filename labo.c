#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils_v2.h"
#include "network.h"

#define NB_ZOMBIES 2
#define BUFFER_SIZE 256

void startZombie(void *arg)
{
    int port = *(int*)arg;
    char portStr[50];
    sprintf(portStr, "%d", port);
    execl("./zombie", "zombie", portStr, NULL);
}

int main(int argc, char const *argv[])
{

    int zombiesPID[NB_ZOMBIES];

    int portZombie1 = tabPorts[randomIntBetween(0, 9)];
    int portZombie2 = tabPorts[randomIntBetween(0, 9)];
    while (portZombie1 == portZombie2)
    {
        portZombie2 = tabPorts[randomIntBetween(0, 9)];
    }

    zombiesPID[0] = fork_and_run1(startZombie, &portZombie1);
    zombiesPID[1] = fork_and_run1(startZombie, &portZombie2);

    char buffer[BUFFER_SIZE];
    printf("CTRL-D pour tuer les zombies\n");
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        buffer[strlen(buffer)] = '\0';
    }

    for (size_t i = 0; i < NB_ZOMBIES; i++)
    {
        skill(zombiesPID[i], SIGINT);
    }
}
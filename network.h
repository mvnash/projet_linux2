
#define SERVER_PORT1 9501
#define SERVER_PORT2 9502
#define SERVER_PORT3 9503
#define SERVER_PORT4 9504
#define SERVER_PORT5 9505
#define SERVER_PORT6 9506
#define SERVER_PORT7 9507
#define SERVER_PORT8 9508
#define SERVER_PORT9 9509
#define SERVER_PORT10 9510

#define NBR_PORTS 10

extern const int tabPorts[NBR_PORTS];

static int* sockFdPortsConnectedTab;

/**
 * PRE: servierIP : a valid IP address
 * POST: on success, test available ports and connects a client socket to serverIP:serverPort ;
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int *testAndConnectPorts(char *ip)

/**
 * PRE:  port: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:port and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port);


/**
 * PRE:  /
 * POST: on success, disconnect all zombies
 */
void disconnectZombies();
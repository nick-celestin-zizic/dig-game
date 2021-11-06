#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define SERVER_PORT 8989
#define BUFFER_SIZE 4096
#define SOCKET_ERROR (-1)
#define SERVER_BACKLOG 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

static inline int
check (int exp, const char *msg) {
  if (exp == SOCKET_ERROR) {
    perror(msg);
    exit(1);
  }

  return exp;
}

static inline void
handle_connection (int client_socket) {
  unsigned char input_data;
  
  size_t bytes_read = read(client_socket, &input_data, sizeof(input_data));

  check(bytes_read, "Read error");

  printf("REQUEST: %u\n", input_data);
}

int
main (void) {
  int server_socket, client_socket, addr_size;
  SA_IN server_addr, client_addr;

  check(server_socket = socket(AF_INET, SOCK_STREAM, 0),
        "Could not create socket");

  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(SERVER_PORT);

  check(bind(server_socket, (SA *) &server_addr, sizeof(server_addr)),
        "Could not bind socket");
  check(listen(server_socket, SERVER_BACKLOG),
        "Server could not listen");

  for (;;) {
    printf("Waiting for connections...\n");
    addr_size = sizeof(SA_IN);
    check(client_socket = accept(server_socket,
                                 (SA *) &client_addr,
                                 (socklen_t *) &addr_size),
          "Accept failed");
    printf("Connected!\n");

    handle_connection(client_socket);
  }
  
  return 0;
}

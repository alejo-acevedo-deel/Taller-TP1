#define _POSIX_C_SOURCE 200112L
#ifndef _SOCKET_H
#define _SOCKET_H

#include <netdb.h>

typedef struct {
  int skt;
  int connection;
  struct addrinfo hints;
  struct addrinfo *ptr;
  int isServer;
} socket_t;

int socket_init(socket_t *self, const char *host, const char *service);

int socket_is_server(socket_t *self);

int socket_listen(socket_t *self, int pool_size);

int socket_accept(socket_t *self);

int socket_connect(socket_t *self);

int socket_receive(socket_t *self, char *buff, int size);

int socket_send(socket_t *self, char *buff, int size);

int socket_uninit(socket_t *self);

#endif

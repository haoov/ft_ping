#ifndef FT_PING
#define FT_PING

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <err.h>
#include <errno.h>
#include <string.h>

/* Error */
#define ERR_NO_HOST		"missing host operand"
#define ERR_UNK_HOST	"unknown host"

struct ft_ping
{
	const char		*hostname;
	int				sockfd;
	struct sockaddr	dst;
};

/* Functions declaration */
struct sockaddr	resolve_hostanme(const char *hostname);
int				create_socket();
void			clean_all(int status, void *data);
void			print_ping(const char *hostname, struct sockaddr dst);

#endif

#ifndef FT_PING_H
#define FT_PING_H

#include "unistd.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "stdio.h"
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>

// Maximum number of host to ping
#define MAX_HOST 16

// Options number
#define OPT_NUM 26

// Max packets to keep timing of
#define	MAX_TIMING_PKT 256

struct ptrlist {
	void			*ptr;
	struct ptrlist	*next;
};

struct opt {
	const char	*lgopt;
	const char	shopt;
	enum {
		boolean,
		number,
		string
	} type;
	union {
		int		intgr;
		double	dbl;
		char	*ptr;
	} val;
};

struct stats {
	struct {
		struct timeval	send_time;
		uint8_t			flags;
	} timing[MAX_TIMING_PKT];

	uint16_t	seq;
	uint64_t	nsend;
	uint64_t	nrecv;
};

struct ping {
	char				*hosts[MAX_HOST];
	uint8_t				host_count;
	struct stats		stats;
	struct opt			opts[OPT_NUM];
	int					socket;
	uint8_t				*sendbuf;
	struct sockaddr_in	addr;
};

void		ping_error(const char *fmt, ...);
void		gc_add(void *ptr);
void		parse_args(int argc, const char **argv);
struct opt	*get_opt(const char *lg, const char sh);
void		init_socket();
void		init_buf();
void		ping_exit();
void		ft_ping();

#endif

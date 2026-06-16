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
#include <netinet/ip.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>

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
		floating,
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

	uint64_t	ttl;

	uint16_t	seq;
	uint64_t	nsend;
	uint64_t	nrecv;

	double		tmin;
	double		tmax;
	double		tsum;
	double		tsumsq;
};

struct ping {
	char				*hosts[MAX_HOST];
	uint8_t				host_count;
	struct stats		stats;
	struct opt			opts[OPT_NUM];
	int					socket;
	uint8_t				*sendbuf;
	uint8_t				*recvbuf;
	struct sockaddr_in	addr;
};

void		ping_error(const char *fmt, ...);
void		gc_add(void *ptr);
void		parse_args(struct ping *p, int argc, const char **argv);
struct opt	*get_opt(struct opt *opts, const char *lg, const char sh);
void		ping_exit();
void		ft_ping(struct ping *p);
uint16_t	compute_cheksum(uint16_t *addr, int count);
void		resolve_host(struct ping *p, const char *host);
void		icmp_echo_request(struct ping *p);
void		icmp_response(struct ping *p);
void		ping_stats(char *host, struct stats s);
void		ping_init(struct ping *p);
void		handle_sigint(int signum);
void		ping_no_args(struct ping *p);

#endif

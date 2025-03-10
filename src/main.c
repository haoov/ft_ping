#include "../inc/ft_ping.h"

int create_socket() {
	int sockfd;

	sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	return sockfd;
}

struct sockaddr resolve_hostanme(const char *hostname) {
	struct addrinfo *res, hint;
	struct sockaddr dst;
	int ret;

	res = NULL;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_RAW;
	hint.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(hostname, NULL, &hint, &res);
	if (ret == -1) {
		errx(EXIT_FAILURE, "%s", gai_strerror(errno));
	}
	if (!res) {
		errx(EXIT_FAILURE, "%s", ERR_UNK_HOST);
	}
	if (memcpy(&dst, res->ai_addr, sizeof(struct sockaddr)) == NULL) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	freeaddrinfo(res);
	return dst;
}

void clean_all(int status, void *data) {
	struct ft_ping *ft_ping;

	(void)status;
	ft_ping = (struct ft_ping*)data;
	if (ft_ping->sockfd) {
		close(ft_ping->sockfd);
	}
}

int main(const int argc, const char **argv) {
	struct ft_ping ft_ping;

	if (argc != 2) {
		errx(EXIT_FAILURE, "%s", ERR_NO_HOST);
	}
	memset(&ft_ping, 0, sizeof(struct ft_ping));
	on_exit(clean_all, &ft_ping);
	ft_ping.sockfd = create_socket();
	ft_ping.dst = resolve_hostanme(argv[1]);
	return EXIT_SUCCESS;
}

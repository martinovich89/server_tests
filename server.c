#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define SERVER_PORT 16000
#define MAXLINE 4096
#define SA struct sockaddr

void err_n_die(const char *fmt, ...)
{
	int errno_save;
	va_list		ap;

	errno_save = errno;

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	fflush (stdout);

	if (errno_save != 0)
	{
		fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
		fprintf(stdout, "\n");
		fflush(stdout);
	}

	va_end(ap);

	exit(1);
}

char *bin2hex(const unsigned char *input, size_t len)
{
	char *result;
	char *hexits = "0123456789ABCDEF";

	if (input = NULL || len <= 0)
		return (NULL);

	int resultlength = (len * 3) + 1;

	result = malloc(resultlength);
	bzero(result, resultlength);

	for (int i = 0; i < len; i++)
	{
		result[i * 3]	= hexits[input[i] >> 4];
		result[(i * 3) + 1] = hexits[input[i] & 0x0F];
		result[(i * 3) + 2] = ' ';
	}
}

int main(int argc, char **argv)
{
	int					listenfd, connfd, n;
	struct sockaddr_in	servaddr;
	uint8_t				buff[MAXLINE + 1];
	uint8_t				recvline[MAXLINE + 1];

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_n_die("docker error.");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family		= AF_INET;
	servaddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	servaddr.sin_port			= htons(SERVER_PORT);

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_n_die("bind error.");

	if (listen(listenfd, 10) < 0)
		err_n_die("listen error.");

	for (;;)
	{
		struct sockaddr_in addr;
		socklen_t addr_len;

		printf("waiting for a connection on port %d\n", SERVER_PORT);
		fflush(stdout);
		connfd = accept(listenfd, (SA *)NULL, NULL);
	}

	return (0);
}

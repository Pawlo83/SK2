#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <fcntl.h>


void main() {
	void setnonblock(int fd) {
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
	
	int sfd, cfd, fdmax, fda, rc, i;
	struct sockaddr_in saddr, caddr;
	static struct timeval timeout;
	fd_set mask, rmask, wmask;
	socklen_t slt = sizeof(caddr);
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(1234);
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setnonblock(sfd);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd, 10);
	FD_ZERO(&mask); FD_ZERO(&rmask); FD_ZERO(&wmask);
	fdmax = sfd;
	
	while(1) {
		FD_SET(sfd, &rmask);
		wmask = mask;
		timeout.tv_sec = 5 * 60; timeout.tv_usec = 0;
		rc = select(fdmax+1, &rmask, &wmask, (fd_set*)0, &timeout);
		if (rc == 0) continue;
		fda = rc;
		if (FD_ISSET(sfd, &rmask)) {
			fda -= 1;
			cfd = accept(sfd, (struct sockaddr*)&caddr, &slt);
			setnonblock(cfd);
			FD_SET(cfd, &mask);
			if (cfd > fdmax) fdmax = cfd;
		}
		for (i = sfd+1; i <= fdmax && fda > 0; i++)
			if (FD_ISSET(i, &wmask)) {
				fda -= 1;
				write(i, "Hello World!\n", 13);
				close(i);
				FD_CLR(i, &mask);
				if (i == fdmax)
				while(fdmax > sfd && !FD_ISSET(fdmax, &mask))
					fdmax -= 1;
			}
	}
}

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
#include <sys/epoll.h>


void main() {
	void setnonblock(int fd) {
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
	
	int sfd, cfd, efd, nfds, i;
	struct sockaddr_in saddr, caddr;
	struct epoll_event event, events[32];
	socklen_t slt = sizeof(caddr);
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(1234);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	setnonblock(sfd);
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd, 10);
	efd = epoll_create1(0);
	event.events = EPOLLIN;
	event.data.fd = sfd;
	epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
	while(1) {
		nfds = epoll_wait(efd, events, 32, -1);
		for (i = 0; i < nfds; i++) {
			if (events[i].data.fd == sfd) {
				cfd = accept(sfd, (struct sockaddr*)&caddr, &slt);
				setnonblock(cfd);
				event.events = EPOLLOUT;
				event.data.fd = cfd;
				epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &event);
			} else {
				write(events[i].data.fd, "Hello World!\n", 13);
				epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
				close(events[i].data.fd);
			}
		}
	}
}

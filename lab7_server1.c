#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char* argv[]){
	int fd, rc;
	socklen_t sl;
	char buf[1024];
	struct sockaddr_in sa, ca;

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(1234);
	fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	bind(fd, (struct sockaddr*)&sa, sizeof(sa));
	while(1) {
		sl = sizeof(ca);
		rc = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&ca, &sl);
		rc = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&ca, &sl);
		write(1, buf, rc);
		sendto(fd, buf, rc, 0, (struct sockaddr*)&ca, sl);
	}
}

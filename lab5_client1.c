#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char* argv[]){
  struct hostent* hostt;
  hostt = gethostbyname(argv[1]);
  
  int sfd, i, rc, bufsize;
  
  char buf[256];
  
  int _read(int sfd, char *buf, int bufsize) {
	int i, rc = 0;
	do {
	i = read(sfd, buf, bufsize);
	if (i < 0) return i;
	bufsize -= i;
	buf += i;
	rc += i;
	} while (*(buf-1)!='\n');
	return rc;
  }
  
  
  struct sockaddr_in saddr;
  sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(atoi(argv[2]));
  //saddr.sin_addr.s_addr = inet_addr(argv[1]);
  
  memcpy(&saddr.sin_addr.s_addr, hostt->h_addr, hostt->h_length);
  
  connect(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  
  write(sfd, argv[3], (strlen(argv[3])));
  write(sfd, "\n", 1);
  bufsize = sizeof(buf);
  _read(sfd,&buf,bufsize);
  write(1, buf, rc);
  
  close(sfd);
}

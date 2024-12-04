#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

 main(int argc, char *argv[]){
    int sfd, rc;
    char buf[256];
    struct sockaddr_in saddr;

    sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[2]));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sfd, (struct sockaddr*) &saddr, sizeof(saddr));

    write(sfd, argv[3], strlen(argv[3]));

    rc = read(sfd, buf, sizeof(buf));
    write(1, buf, rc);

    close(sfd);
}

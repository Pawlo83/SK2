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

int main() {
    void setnonblock(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    int sfd, cfd, fdmax, fda, rc, i, xyz;
    long unsigned int j;
    char buf[256];
    int XY[10] = {0,0,0,0,0,0,0,0,0,0}, YX[10] = {0,0,0,0,0,0,0,0,0,0};

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
    FD_ZERO(&mask);
    FD_ZERO(&rmask);
    FD_ZERO(&wmask);
    fdmax = sfd;

    while (1) {
        FD_SET(sfd, &rmask);

        timeout.tv_sec = 5 * 60;
        timeout.tv_usec = 0;
        rc = select(fdmax + 1, &rmask, &wmask, (fd_set*)0, &timeout);
        if (rc == 0) continue;
        fda = rc;

        if (FD_ISSET(sfd, &rmask)) {
            fda -= 1;
            cfd = accept(sfd, (struct sockaddr*)&caddr, &slt);
            setnonblock(cfd);
            FD_SET(cfd, &rmask);
            if (cfd > fdmax) fdmax = cfd;
            sleep(10);
        } else {
            for (i = sfd + 1; i <= fdmax && fda > 0; i++) {
                if (FD_ISSET(i, &rmask)) {
                    fda -= 1;
                    xyz = read(i, buf, sizeof(buf));
                    write(1, buf, xyz);
                    printf("\n");

                    if ((strncmp(buf, "123456", 10)) == 0) {
                        for (j = 0; j < (sizeof(YX) / sizeof(YX[0])); j++) {
                            if (YX[j] == 0) {
                                YX[j] = i;
                                break;
                            }
                        }
                    } else {
                        if ((strncmp(buf, "654321", 10)) == 0) {
                            for (j = 0; j < (sizeof(XY) / sizeof(XY[0])); j++) {
                                if (XY[j] == 0) {
                                    XY[j] = i;
                                    break;
                                }
                            }
                        }
                    }

                    FD_SET(i, &wmask);
                    FD_CLR(i, &rmask);
                    break;

                    if (i == fdmax)
                        while (fdmax > sfd && !FD_ISSET(fdmax, &wmask) && !FD_ISSET(fdmax, &rmask))
                            fdmax -= 1;
                } else {
                    if (FD_ISSET(i, &wmask)) {
                        fda -= 1;
                        int tempXY = 0, tempYX = 0;

                        for (j = 0; j < (sizeof(XY) / sizeof(XY[0])); j++) {
                            if (XY[j] == i) {
                                tempXY = 1;
                                XY[j] = 0;
                            }
                        }

                        for (j = 0; j < (sizeof(YX) / sizeof(YX[0])); j++) {
                            if (YX[j] == i) {
                                tempYX = 1;
                                YX[j] = 0;
                            }
                        }

                        if (tempYX == 1) {
                            write(i, "Y X\n", 5);
                        } else {
                            if (tempXY == 1) {
                                write(i, "X Y\n", 5);
                            } else {
                                write(i, "ERROR\n", 7);
                            }
                        }

                        close(i);
                        FD_CLR(i, &wmask);

                        if (i == fdmax)
                            while (fdmax > sfd && !FD_ISSET(fdmax, &wmask) && !FD_ISSET(fdmax, &rmask))
                                fdmax -= 1;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

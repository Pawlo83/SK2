#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void main(int argc, char* argv[]){
	int sfd, cfd, rc = 0;
	char buf[1024];
	socklen_t sl;
	struct sockaddr_in sa, ca;
	SSL_CTX* ctx;
	SSL* ssl;
	SSL_load_error_strings();
	SSL_library_init();
	ctx = SSL_CTX_new(TLS_server_method());
	SSL_CTX_use_certificate_file(ctx, "server.crt",
	SSL_FILETYPE_PEM);
	SSL_CTX_use_PrivateKey_file(ctx, "server.key",
	SSL_FILETYPE_PEM);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(1234);
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sfd, (struct sockaddr*)&sa, sizeof(sa));
	listen(sfd, 10);
	while(1) {
		sl = sizeof(ca);
		cfd = accept(sfd, (struct sockaddr*)&ca, &sl);
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, cfd);
		SSL_accept(ssl);
		rc = SSL_read(ssl, buf, sizeof(buf));
		SSL_write(ssl, "Welcome to the SSL/TLS server!\n", 32);
		SSL_shutdown(ssl); SSL_free(ssl); close(cfd);
	}
	SSL_CTX_free(ctx); close(sfd); return EXIT_SUCCESS;
}

#ifndef TCP__
#define TCP__

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>

int serv(int port) {
	signal(SIGCHLD, SIG_IGN);
	int s = socket(PF_INET, SOCK_STREAM, 0), r = 1;
	if(s == -1 || setsockopt(s, SOL_SOCKET,
				SO_REUSEADDR, &r, sizeof(r))
			|| bind(s, (void*) &(struct sockaddr_in) {
				.sin_family = AF_INET,
				.sin_port = htons(port), },
				sizeof(struct sockaddr_in))
			|| listen(s, 10)) {
		perror("serv()"); exit(1); }
	while((r = accept(s, 0, 0)) && fork());
	return r; }

typedef struct { void (*ls)(int); int req; } pservht_;
void* pservh_(pservht_* hand) { hand->ls(hand->req); return 0; }

void pserv(int port, void (*ls)(int)) {
	signal(SIGCHLD, SIG_IGN);
	int s = socket(PF_INET, SOCK_STREAM, 0), r = 1;
	if(s == -1 || setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r))
			|| bind(s, (void*) &(struct sockaddr_in) {
				.sin_family = AF_INET, .sin_port = htons(port) },
				sizeof(struct sockaddr_in))
			|| listen(s, 10)) {
		perror("pserv()"); exit(1); }
	while((r = accept(s, 0, 0)) > 0) {
		pservht_* hand = malloc(sizeof(pservht_));
		*hand = (pservht_) { ls, r };
		pthread_create(&(pthread_t) { 0 }, 0, (void*) &pservh_, hand); }}

int con(char* host, int port) {
	int s = socket(PF_INET, SOCK_STREAM, 0);
	struct addrinfo* a;
	if(s == -1 || getaddrinfo(host, 0, 0, &a)
			|| connect(s, (void*)
				&(struct sockaddr_in) {
				.sin_family = AF_INET,
				.sin_port = htons(port),
				.sin_addr =
				((struct sockaddr_in*)(void*)
				 a->ai_addr)->sin_addr },
				sizeof(struct sockaddr_in))) {
		perror("con()"); exit(1); }
	return s; }

int end(int s) {
	return shutdown(s, SHUT_RDWR) ?: close(s); }

#endif

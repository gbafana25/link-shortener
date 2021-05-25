#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "storage.h"
#include "url.h"

#define PORT 8080
#define BUF_SIZE 512
#define SHURL_SIZE 10 // shortened url size
#define STORE_FILE "records.txt"


void getshortenedurl(char *url, char *filename, int recv) {
	char *template_response = "HTTP/1.1 301 Moved Permanently\r\nLocation: ";
	char *start = strstr(url, "go/");
	char *beg = strchr(start, '/');
	char *end = strchr(beg, ' ');
	char finished[end-(beg+1)];
	strncpy(finished, beg+1, end-(beg+1));
	finished[sizeof(finished)] = '\0';
	FILE *storage;
	storage = fopen(filename, "r");
	char *line;
	size_t size = 0;
	while(getline(&line, &size, storage) != -1) {
		char *s = strchr(line, '{');
		char *e = strchr(s, '}');
		char shorten[e-(s+1)];
		strncpy(shorten, s+1, e-(s+1));
		shorten[sizeof(shorten)] = '\0';
		if(strcmp(finished, shorten) == 0) {
			int source_size = (int) (s-line);
			char source_url[source_size];
			char *response[sizeof(template_response) + sizeof(source_url) + 1];
			memset(&source_url, 0, sizeof(source_url));
			memset(&response, 0, sizeof(response));
			strncpy(source_url, line, source_size);
			strncat((char * restrict) &response, template_response, strlen(template_response));
			strncat((char * restrict) &response, source_url, source_size);
			strncat((char * restrict) &response, "\r\n", 2);
			send(recv, (char * restrict) &response, strlen((const char *) &response), 0);
			//printf("%s\n", response);
			/*
			strcat(response, "\0");
			printf("%s\n", response);
			//return (const char *) &source_url;
			*/
		}
	}
}

int main() {
	int srv, client;
	char *data[BUF_SIZE];
	struct sockaddr_in srvaddr, clientaddr;
	char *create_request = "GET /create/";
	char *access_request = "GET /go/";
	//char *testr_size = (char *) sizeof(test_response);
	//strncat(test_response, (const char *) &tesr_size, (sizeof(test_response)
	
	//char *rtmsg = "got link";

	srv = socket(AF_INET, SOCK_STREAM, 0);
	if(srv == -1) {
		perror("failed to create socket");
		exit(1);
	}

	int opt = 1;
	setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
	memset(&srvaddr, 0, sizeof(srvaddr));	
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	

	if(bind(srv, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) == -1) {
		perror("failed to bind");
		exit(1);
	}
	
	if(listen(srv, 5) == -1) {
		perror("failed to start listening");
		exit(1);
	}

	memset(&clientaddr, 0, sizeof(clientaddr));
	int srv_size = sizeof(srvaddr);
	while(1) {
		client = accept(srv, (struct sockaddr *) &srvaddr, &srv_size);
		if(client == -1) {
			perror("client failed to connect");
			exit(1);
		} else {
			recv(client, data, BUF_SIZE, 0);
			char *base[1000];
			strncpy((char * restrict) &base, (const char * restrict) &data, strlen(create_request));
			//strncpy((char * restrict) &acc_comp, (const char * restrict) &data, strlen(access_request));
			if(strncmp((const char *) &base, create_request, strlen(create_request)) == 0) {
				parse_url(data, "create/", STORE_FILE);
				get_short_link(SHURL_SIZE, STORE_FILE, client);
				close(client);
			}
			else if(strncmp((const char *) &base, access_request, strlen(access_request)) == 0) {
			 	getshortenedurl((char *) &data, STORE_FILE, client);
				close(client);
			}
		}
	}
}

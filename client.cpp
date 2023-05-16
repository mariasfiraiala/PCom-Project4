#include <bits/stdc++.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "parser.h"
#include "utils.h"
#include "helpers.h"
#include "commands.h"

int main(void) {
    int sockfd;

    char *login_cookie = NULL;
    char *jwt_token = NULL;
    char buff[LINELEN];

    while (1) {
        sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);

        memset(buff, 0, LINELEN);
        fgets(buff, LINELEN, stdin);

        char argc, *argv[LINELEN];
        argc = parse_by_whitespace(buff, argv);

        if (argc > 1)
            printf("Invalid command.\n");

        if (!strcmp(argv[0], "register")) {
            auth(sockfd);
        } else if (!strcmp(argv[0], "login")) {
            login_cookie = login(sockfd);
        } else if (!strcmp(argv[0], "enter_library")) {
            jwt_token = enter_library(sockfd, login_cookie);
        } else if (!strcmp(argv[0], "get_books")) {
            get_books(sockfd, jwt_token);
        } else if (!strcmp(argv[0], "get_book")) {
            get_book(sockfd, jwt_token);
        } else if (!strcmp(argv[0], "add_book")) {
            add_book(sockfd, jwt_token);
        } else if (!strcmp(argv[0], "delete_book")) {
            delete_book(sockfd, jwt_token);
        } else if (!strcmp(argv[0], "logout")) {

        } else if (!strcmp(argv[0], "exit")) {
            break;
        } else {
            printf("Invalid command.\n");
        }

        close(sockfd);
    }

    close(sockfd);
    return 0;
}
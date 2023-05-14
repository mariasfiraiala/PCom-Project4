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
    sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);

    char buff[LINELEN];

    while (1) {
        memset(buff, 0, LINELEN);
        fgets(buff, LINELEN, stdin);

        char argc, *argv[LINELEN];
        argc = parse_by_whitespace(buff, argv);

        if (argc > 1)
            printf("Invalid command.\n");

        if (!strcmp(argv[0], "register")) {
            auth(sockfd);
        } else if (!strcmp(argv[0], "login")) {

        } else if (!strcmp(argv[0], "enter_library")) {

        } else if (!strcmp(argv[0], "get_books")) {

        } else if (!strcmp(argv[0], "get_book")) {

        } else if (!strcmp(argv[0], "add_book")) {

        } else if (!!strcmp(argv[0], "delete_book")) {

        } else if (!strcmp(argv[0], "logout")) {

        } else if (!strcmp(argv[0], "exit")) {
            return 0;
        } else {
            printf("Invalid command.\n");
        }
    }
    return 0;
}
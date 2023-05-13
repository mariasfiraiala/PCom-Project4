#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "parser.h"
#include "utils.h"


void auth() {
    char username[LINELEN], password[LINELEN], buff[LINELEN];
    char argc, *argv[LINELEN];

    while (1) {
        printf("username=");
        fgets(buff, LINELEN, stdin);

        argc = parse_by_whitespace(buff, argv);
        if (argc != 1) {
            printf("Whitespaces are not permited in your username\n");
        } else {
            strcpy(username, argv[0]);
            break;
        }
    }

    while (1) {
        printf("password=");
        fgets(buff, LINELEN, stdin);

        argc = parse_by_whitespace(buff, argv);
        if (argc != 1) {
            printf("Whitespaces are not permited in your password\n");
        } else {
            strcpy(password, argv[0]);
            break;
        }
    }

    printf("%s %s\n", username, password);
}

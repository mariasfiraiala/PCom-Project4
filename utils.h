#ifndef __UTILS_H__
#define __UTILS_H__

#include <errno.h>

#define BUFLEN 4096
#define LINELEN 1024

#define REGISTER    "/api/v1/tema/auth/register"
#define LOGIN       "/api/v1/tema/auth/login"
#define LOGOUT      "/api/v1/tema/auth/logout"
#define LIBRARY     "/api/v1/tema/library/access"
#define BOOKS       "/api/v1/tema/library/books"
#define BOOK        "/api/v1/tema/library/books/"

#define PAYLOAD "application/json"
#define SERVER "34.254.242.81:8080"
#define IP "34.254.242.81"
#define PORT 8080

#define DIE(assertion, call_description)                                       \
    do {                                                                       \
        if (assertion) {                                                       \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
            perror(call_description);                                          \
            exit(errno);                                                       \
        }                                                                      \
    } while (0)

#endif

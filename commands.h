#ifndef __COMMANDS_H__
#define __COMMANDS_H__

void auth(int sockfd);

void login(int sockfd, char **cookie);

char *enter_library(int sockfd, char *cookie);

void get_books(int sockfd, char *token);

void get_book(int sockfd, char *token);

void add_book(int sockfd, char *token);

void delete_book(int sockfd, char *token);

void logout(int sockfd, char **cookie, char **token);

#endif
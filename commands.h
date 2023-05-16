#ifndef __COMMANDS_H__
#define __COMMANDS_H__

void auth(int sockfd);

char *login(int sockfd);

char *enter_library(int sockfd, char *cookie);

void get_books(int sockfd, char *token);

void get_book(int sockfd, char *token);

void add_book(int sockfd, char *token);

void delete_book(int sockfd, char *token);

#endif
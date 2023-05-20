#include <bits/stdc++.h>

#include <stdio.h>
#include <string.h>

#include <nlohmann/json.hpp>

#include "commands.h"
#include "requests.h"
#include "helpers.h"
#include "parser.h"
#include "utils.h"

using json = nlohmann::json;

void auth(int sockfd) {
    char username[LINELEN], password[LINELEN], buff[LINELEN];
    char argc, *argv[LINELEN];

    /**
     * Get username in a loop until it's satisfactory
     * (it doesn't have whitespaces)
     */
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

    /**
     * Get password in a loop until it's satisfactory
     * (it doesn't have whitespaces)
     */
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

    /**
     * Construct json object and transform it into an array of chars
     * in order to be inserted into the POST HTTP request
     */
    json auth = {{"username", username}, {"password", password}};
 
    char *auth_string = (char *)malloc(auth.dump().length() + 1);
    DIE(!auth_string, "malloc() failed");
    strcpy(auth_string, auth.dump().c_str());

    char *request = compute_post_request(SERVER, REGISTER, PAYLOAD,
                                         auth_string, NULL, NULL);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    /* Kinda parse the response to check for username error */
    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Succesfully registered user %s.\n", username);
    }

    free(response);
    free(auth_string);
}

void login(int sockfd, char **cookie) {
    char username[LINELEN], password[LINELEN], buff[LINELEN];
    char argc, *argv[LINELEN];

    /**
     * Get username in a loop until it's satisfactory
     * (it doesn't have whitespaces)
     */
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

    /**
     * Get password in a loop until it's satisfactory
     * (it doesn't have whitespaces)
     */
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

    /**
     * Construct json object and transform it into an array of chars
     * in order to be inserted into the POST HTTP request
     */
    json auth = {{"username", username}, {"password", password}};

    char *auth_string = (char *)malloc(auth.dump().length() + 1);
    DIE(!auth_string, "malloc() failed");
    strcpy(auth_string, auth.dump().c_str());

    char *request = compute_post_request(SERVER, LOGIN, PAYLOAD,
                                         auth_string, NULL, NULL);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    /* Kinda parse the response to check for username error */
    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        char *tmp = strtok(strstr(response, "connect.sid"), ";");
        free(*cookie);
        *cookie = (char *)malloc(strlen(tmp) + 1);
        DIE(!(*cookie), "malloc() failed");
        memcpy(*cookie, tmp, strlen(tmp) + 1);

        printf("Successfully logged in user %s.\n", username);
    }

    free(auth_string);
    free(response);
}

char *enter_library(int sockfd, char* cookie) {
    char *request = compute_get_request(SERVER, LIBRARY, cookie, NULL);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    /**
     * This call will ALWAYS return a non-null value due to the
     * enter_library API providing a json response for error and
     * non-error returns
     */
    json r_enter = json::parse(strstr(response, "{"));
    char *token  = NULL;

    if (r_enter.contains("error")) {
        std::cout << r_enter.value("error", "") << "\n";
    } else {
        std::string tmp;
        r_enter.at("token").get_to(tmp);

        token = (char *)malloc(tmp.length() + 1);
        DIE(!token, "malloc() failed");
        strcpy(token, tmp.c_str());

        printf("Successfully entered the library.\n");
    }

    free(response);

    return token;
}

void get_books(int sockfd, char *token) {
    char *request = compute_get_request(SERVER, BOOKS, NULL, token);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    if (strstr(response, "[")) {
        std::cout << json::parse(strstr(response, "[")).dump(4) << "\n";
    } else {
        json r_get = json::parse(strstr(response, "{"));
        std::cout << r_get.value("error", "") << "\n";
    }

    free(response);
}

void get_book(int sockfd, char *token) {
    char id[LINELEN] = { 0 }, buff[LINELEN];

    printf("id=");
    fgets(buff, LINELEN, stdin);
    memcpy(id, buff, strlen(buff) - 1);

    char *path = (char *)malloc(strlen(BOOK) + strlen(id) + 1);
    DIE(!path, "malloc() failed");
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = compute_get_request(SERVER, path, NULL, token);
    send_to_server(sockfd, request);
    free(path);
    free(request);

    char *response = receive_from_server(sockfd);

    /**
     * This call will ALWAYS return a non-null value due to the
     * get_book API providing a json response for error and
     * non-error returns
     */
    json r_get = json::parse(strstr(response, "{"));
    if (r_get.contains("error")) {
        std::cout << r_get.value("error", "") << "\n";
    } else {
        std::cout << r_get.dump(4) << "\n";
    }

    free(response);
}

bool is_int(char *buff) {
    for (; *buff; ++buff)
        if (!isdigit(*buff))
            return false;

    return true;
}

void add_book(int sockfd, char *token) {
    char buff[LINELEN];
    char title[LINELEN] = { 0 }, author[LINELEN] = { 0 }, genre[LINELEN] = { 0 };
    char publisher[LINELEN] = { 0 }, page_count[LINELEN] = { 0 };

    printf("title=");
    fgets(buff, LINELEN, stdin);
    memcpy(title, buff, strlen(buff) - 1);

    printf("author=");
    fgets(buff, LINELEN, stdin);
    memcpy(author, buff, strlen(buff) - 1);

    printf("genre=");
    fgets(buff, LINELEN, stdin);
    memcpy(genre, buff, strlen(buff) - 1);

    printf("publisher=");
    fgets(buff, LINELEN, stdin);
    memcpy(publisher, buff, strlen(buff) - 1);

    while (1) {
        printf("page_count=");
        fgets(buff, LINELEN, stdin);
        strtok(buff, "\n");

        if (!is_int(buff)) {
            printf("Page count should be an int.\n");
        } else {
            memcpy(page_count, buff, strlen(buff));
            break;
        }
    }

    json add = {{"title", title}, {"author", author}, {"genre", genre},
        {"page_count", atoi(page_count)}, {"publisher", publisher}};

    char *add_string = (char *)malloc(add.dump().length() + 1);
    DIE(!add_string, "malloc() failed");
    strcpy(add_string, add.dump().c_str());

    char *request = compute_post_request(SERVER, BOOKS, PAYLOAD,
                                         add_string, NULL, token);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Successfully added book %s.\n", title);
    }

    free(add_string);
    free(response);
}

void delete_book(int sockfd, char *token) {
    char id[LINELEN] = { 0 }, buff[LINELEN];

    printf("id=");
    fgets(buff, LINELEN, stdin);
    memcpy(id, buff, strlen(buff) - 1);

    char *path = (char *)malloc(strlen(BOOK) + strlen(id) + 1);
    DIE(!path, "malloc() failed");
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = compute_delete_request(SERVER, path, NULL, token);
    send_to_server(sockfd, request);
    free(path);
    free(request);

    char *response = receive_from_server(sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Successfully deleted book %s.\n", id);
    }

    free(response);
}

void logout(int sockfd, char **cookie, char **token) {
    char *request = compute_get_request(SERVER, LOGOUT, *cookie, NULL);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    } else {
        printf("Successfully logged out user.\n");
    }

    free(*cookie);
    free(*token);
    *cookie = NULL;
    *token  = NULL;
    free(response);
}

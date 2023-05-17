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
    json auth = {
        {"username", username},
        {"password", password}
    };

    char *auth_string = new char [(auth.dump()).length() + 1];
    strcpy(auth_string, auth.dump().c_str());

    char *request = compute_post_request(SERVER, REGISTER, PAYLOAD, auth_string, 1, NULL, 0, NULL);
    // TODO: Remove this
    printf("%s\n", request);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    // TODO: Remove this
    printf("Response is %s\n", response);

    /* Kinda parse the response to check for username error */
    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    }

    free(response);
    delete auth_string;
}

char *login(int sockfd) {
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
    json auth = {
        {"username", username},
        {"password", password}
    };

    char *auth_string = new char [(auth.dump()).length() + 1];
    strcpy(auth_string, auth.dump().c_str());

    char *request = compute_post_request(SERVER, LOGIN, PAYLOAD, auth_string, 1, NULL, 0, NULL);
    // TODO: Remove this
    printf("%s\n", request);
    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);

    // TODO: Remove this
    printf("Response is %s\n", response);

    char *cookie;
    /* Kinda parse the response to check for username error */
    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
        cookie = NULL;
    } else {
        cookie = strtok(strstr(response, "connect.sid"), ";");
    }

    delete auth_string;
    free(response);

    return cookie;
}

char *enter_library(int sockfd, char *cookie) {
    char *request = compute_get_request(SERVER, LIBRARY, NULL, &cookie, (cookie != NULL), NULL);
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

    /**
     * This call will ALWAYS return a non-null value due to the
     * enter_library API providing a json response for error and
     * non-error returns
     */
    json r_enter = json::parse(strstr(response, "{"));
    char *token = NULL;

    if (r_enter.contains("error")) {
        std::cout << r_enter.value("error", "") << "\n";
    } else {
        std::string tmp;
        r_enter.at("token").get_to(tmp);

        token = new char[tmp.length() + 1];
        strcpy(token, tmp.c_str());
    }

    free(response);

    return token;
}

void get_books(int sockfd, char *token) {
    char *request = compute_get_request(SERVER, BOOKS, NULL, NULL, 0, token);
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

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

    char *path = new char[strlen(BOOK) + strlen(id) + 1];
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = compute_get_request(SERVER, path, NULL, NULL, 0, token);
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

    /**
     * This call will ALWAYS return a non-null value due to the
     * get_book API providing a json response for error and
     * non-error returns
     */
    json r_get = json::parse(strstr(response, "{"));
    if (r_get.contains("error")) {
        std::cout << r_get.value("error", "") << "\n";
    } else {
         std::cout << r_get.dump() << "\n";
    }

    free(response);
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

    printf("page_count=");
    fgets(buff, LINELEN, stdin);
    memcpy(page_count, buff, strlen(buff) - 1);

    json add = {
        {"title", title},
        {"author", author},
        {"genre", genre},
        {"page_count", atoi(page_count)},
        {"publisher", publisher}
    };

    char *add_string = new char [(add.dump()).length() + 1];
    strcpy(add_string, add.dump().c_str());

    char *request = compute_post_request(SERVER, BOOKS, PAYLOAD, add_string, 1, NULL, 0, token);
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    }

    free(response);
}

void delete_book(int sockfd, char *token) {
    char id[LINELEN] = { 0 }, buff[LINELEN];

    printf("id=");
    fgets(buff, LINELEN, stdin);
    memcpy(id, buff, strlen(buff) - 1);

    char *path = new char[strlen(BOOK) + strlen(id) + 1];
    strcpy(path, BOOK);
    strcat(path, id);

    char *request = compute_delete_request(SERVER, path, NULL, NULL, 0, token);
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    }

    free(response);
}

void logout(int sockfd, char **cookie, char **token) {
    char *request = compute_get_request(SERVER, LOGOUT, NULL, cookie,  (*cookie != NULL), NULL);
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

    if (strstr(response, "{")) {
        json r_auth = json::parse(strstr(response, "{"));
        std::cout << r_auth.value("error", "") << "\n";
    }

    *cookie = NULL;
    *token = NULL;
    free(response);
}

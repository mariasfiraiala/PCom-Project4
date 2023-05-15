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

    char *request = compute_post_request(SERVER, REGISTER, PAYLOAD, auth_string, 1, NULL, 0);
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

    char *request = compute_post_request(SERVER, LOGIN, PAYLOAD, auth_string, 1, NULL, 0);
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
    char *request = compute_get_request(SERVER, LIBRARY, NULL, &cookie, (cookie != NULL));
    // TODO: Remove this
    printf("%s\n", request);

    send_to_server(sockfd, request);
    free(request);

    char *response = receive_from_server(sockfd);
    // TODO: Remove this
    printf("Response is %s\n", response);

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
    
}

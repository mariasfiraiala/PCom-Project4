#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "requests.h"
#include "helpers.h"
#include "utils.h"

char *compute_get_request(const char *host, const char *url, char *query_params,
						  char **cookies, int cookies_count,
						  char *token) {
	char *message = (char *)calloc(BUFLEN, sizeof(char));
	DIE(!message, "calloc() failed");

	char *line = (char *)calloc(LINELEN, sizeof(char));
	DIE(!line, "calloc() failed");

	/* Write the method name, URL, request params (if any) and protocol type */
	if (query_params != NULL) {
		sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "GET %s HTTP/1.1", url);
	}

	compute_message(message, line);

	/* Add the host */
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	/* Add headers and/or cookies, according to the protocol format */
	if (cookies_count) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");
		for (int i = 0; i < cookies_count - 1; i++) {
			strcat(line, cookies[i]);
			strcat(line, "; ");
		}

		strcat(line, cookies[cookies_count - 1]);
		compute_message(message, line);
	}

	if (token) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
	}

	/* Add final new line */
	compute_message(message, "");
	free(line);

	return message;
}

char *compute_post_request(const char *host, const char *url, const char *content_type,
						   const char *body_data, int body_data_fields_count,
						   char **cookies, int cookies_count,
						   char *token) {
	char *message = (char *)calloc(BUFLEN, sizeof(char));
	DIE(!message, "calloc() failed");

	char *line = (char *)calloc(LINELEN, sizeof(char));
	DIE(!line, "calloc() failed");

	char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));
	DIE(!body_data_buffer, "calloc() failed");

	/* Write the method name, URL and protocol type */
	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);

	/* Add the host */
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	/**
	 * Add necessary headers (Content-Type and Content-Length are mandatory)
	 * In order to write Content-Length we must first compute the
	 * message size
	 */
	memset(body_data_buffer, 0, LINELEN);
	strcat(body_data_buffer, body_data);

	sprintf(line, "Content-Type: %s", content_type);
	compute_message(message, line);

	sprintf(line, "Content-Length: %lu", strlen(body_data_buffer));
	compute_message(message, line);

	/* Add cookies */
	if (cookies) {
		memset(line, 0, LINELEN);
		strcat(line, "Cookie: ");
		for (int i = 0; i < cookies_count - 1; i++) {
			strcat(line, cookies[i]);
			strcat(line, ";");
		}

		strcat(line, cookies[cookies_count - 1]);
		compute_message(message, line);
	}

	if (token) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
	}

	/* Add new line at end of header */
	compute_message(message, "");

	/* Add the actual payload data */
	memset(line, 0, LINELEN);
	strcat(message, body_data_buffer);

	free(line);
	free(body_data_buffer);

	return message;
}

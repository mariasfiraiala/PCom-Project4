#ifndef _HELPERS_
#define _HELPERS_

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void compute_message(char *message, const char *line);

int open_connection(const char *host_ip, int portno, int ip_type,
                    int socket_type, int flag);

void close_connection(int sockfd);

void send_to_server(int sockfd, char *message);

char *receive_from_server(int sockfd);

char *basic_extract_json_response(char *str);

#endif

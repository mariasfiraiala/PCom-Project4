#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(const char *host, const char *url,
						  char *cookies, char *token);

char *compute_post_request(const char *host, const char *url,
						   const char *content_type, const char *body_data,
						   char *cookies, char *token);

char *compute_delete_request(const char *host, const char *url,
							 char *cookies, char *token);

#endif

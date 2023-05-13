#ifndef __PARSER_H__
#define __PARSER_H__

int parse_by_whitespace(char *buf, char **argv);

char *argv_to_string(char **argv, int start, int end);

#endif
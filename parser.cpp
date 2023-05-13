#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int parse_by_whitespace(char *buf, char **argv) {
	int argc = 0;
	for (char *p = strtok(buf, " \t\n"); p; p = strtok(NULL, " \t\n"))
		argv[argc++] = p;

	return argc;
}

char *argv_to_string(char **argv, int start, int end) {
	static char str[BUFSIZ];
	strcpy(str, "");

	if (start >= end)
		return NULL;

	strcpy(str, argv[start]);

	for (int index = start + 1; index < end; ++index) {
		strcat(str, " ");
		strcat(str, argv[index]);
	}

	return str;
}

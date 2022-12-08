#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this function finds the index of the marker that has four distinct chars.
int strmkr(char* s, size_t n) {
	if (strlen(s) < 4) {
		return -1;
	}
	for (size_t i = n-1; i < strlen(s); i++) {
		int count = 0;
		for (size_t j = 0; j < n-1; j++) {
			for (size_t k = j + 1; k < n; k++) {
				if (s[i - j] == s[i - k]) {
					goto next;
				}
			}
		}
		return i+1;
next:
		continue;
	}
	return -1;
}

int main(int argc, char *argv[]) {
	// read from stdin
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while((read = getline(&line, &len, stdin)) != -1) {
		printf("%d\n", strmkr(line, 14));
	}
	return 0;
}


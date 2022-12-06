#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char* debug;

#define CHARNUM 64
#define CHARCODE(ch) ((ch) > 'a' ? (ch) - 'a' + 1: (ch) - 'A' + 27)

typedef struct state {
	char chars[CHARNUM];
	int32_t sum;
} state;


int32_t getsum(state* s) {
	return s->sum;
}

int run(state* s, char* input) {
	size_t len = strlen(input);
	size_t j = 0;
	memset(s->chars, 0, CHARNUM * sizeof(char));
	for (size_t i = 0; i < (len-1)/2; i++) {
		int code = CHARCODE(input[i]);
		s->chars[code] = 1;
	}
	for (size_t i = (len-1)/2; i < len-1; i++) {
		int code = CHARCODE(input[i]);
		if (s->chars[code]) {
			s->sum += code;
			return 0;
		}
	}
	return 1;
}

int main (int argc, char** argv) {
	debug = getenv("DEBUG");

	// abort the program if the user did not provide a filename
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[1]);
		exit(1);
	}

	// read file
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Could not open file %s\n", argv[1]);
		exit(1);
	}

	// initialize state
	state *s = &((state) {{0}, 0});

	// read the file line by line
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, f)) != -1) {
		if (run(s, line) != 0) {
			goto finalize;
		}
	}

	printf("%d\n", getsum(s));

finalize:
	fclose(f);

	return 1;
}


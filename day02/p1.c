#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROCK 1
#define PAPER 2
#define SCISSORS 3

#define LOSE 0
#define DRAW 3
#define WIN 6

char* debug;

typedef struct state {
	int32_t score;
} state;

int check_rock_paper_scissor(int32_t a, int32_t b) {
	if (a == b) {
		return DRAW;
	} else if (a == ROCK && b == SCISSORS) {
		return LOSE;
	} else if (a == PAPER && b == ROCK) {
		return LOSE;
	} else if (a == SCISSORS && b == PAPER) {
		return LOSE;
	} else {
		return WIN;
	}
}

int parse(char s) {
	return s >= 'X' ? s - 'X' + 1 : s - 'A' + 1;
}

int run_strategy(state* s, char *input) {
	size_t i = 0;
	if (strlen(input) < 3) {
		return 1;
	}

	int left = parse(input[0]);
	int right = parse(input[2]);

	s->score += (int32_t) right;
	s->score += check_rock_paper_scissor(left, right);

	return 0;
}

int32_t getscore(state* s) {
	return s->score;
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
	state *s = &((state) {0});

	// read the file line by line
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, f)) != -1) {
		if (run_strategy(s, line) != 0) {
			goto finalize;
		}
	}

	printf("%d\n", getscore(s));

finalize:
	fclose(f);

	return 1;
}

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct state {
	// accumulated calories for the current group;
	int32_t acccal;

	// top three maximum calories for all the groups;
	int32_t maxcal[3];
} state;

char* debug = NULL;

void setmax(state *s) {
	if (s->acccal <= s->maxcal[2]) {
		return;
	}
	if (s->acccal <= s->maxcal[1]) {
		s->maxcal[2] = s->acccal;
		if (debug != 0) {
			printf("New maxcal: %d %d %d\n", s->maxcal[0], s->maxcal[1], s->maxcal[2]);
		}
		return;
	}
	if (s->acccal <= s->maxcal[0]) {
		s->maxcal[2] = s->maxcal[1];
		s->maxcal[1] = s->acccal;
		if (debug != 0) {
			printf("New maxcal: %d %d %d\n", s->maxcal[0], s->maxcal[1], s->maxcal[2]);
		}
		return;
	}
	s->maxcal[2] = s->maxcal[1];
	s->maxcal[1] = s->maxcal[0];
	s->maxcal[0] = s->acccal;
	if (debug != 0) {
		printf("New maxcal: %d %d %d\n", s->maxcal[0], s->maxcal[1], s->maxcal[2]);
	}
}

int32_t getmax(state *s) {
	return s->maxcal[0] + s->maxcal[1] + s->maxcal[2];
}

// This function accumulates the calories for the current group
void accumulate(state *s, int32_t cal) {
	if (cal == 0) {
		setmax(s);
		s->acccal = 0;
	} else {
		s->acccal += cal;
	}
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
	state *s = &((state) {0, {0, 0, 0}});

	// read the file line by line
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, f)) != -1) {
		// convert the line to an integer
		int32_t cal = atoi(line);

		// accumulate the calories
		accumulate(s, cal);
	}

	printf("%d\n", getmax(s));

	return 1;
}


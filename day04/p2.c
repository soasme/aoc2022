#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char* debug;


typedef struct state {
	int32_t count;
} state;

int32_t getcount(state* s) {
	return s->count;
}

void parse(char* input, int* s1, int* e1, int* s2, int* e2) {
	char buf[128] = "";
	strcpy(buf, input);
	char *dash = "-";
	char *comma = ",";
	char *left = strtok(buf, comma);
	char *right = strtok(NULL, comma);
	*s1 = atoi(strtok(left, dash));
	*e1 = atoi(strtok(NULL, dash));
	*s2 = atoi(strtok(right, dash));
	*e2 = atoi(strtok(NULL, dash));
}

int is_started(int s1, int e1, int s2, int e2) {
	return (s1 == s2 && e1 < e2);
}

int is_during(int s1, int e1, int s2, int e2) {
	return (s1 > s2 && e1 < e2);
}

int is_finished(int s1, int e1, int s2, int e2) {
	return (s1 > s2 && e1 == e2);
}

int is_equal(int s1, int e1, int s2, int e2) {
	return (s1 == s2 && e1 == e2);
}

int is_finished_by(int s1, int e1, int s2, int e2) {
	return (s1 < s2 && e1 == e2);
}

int is_contained(int s1, int e1, int s2, int e2) {
	return (s1 < s2 && e1 > e2);
}

int is_started_by(int s1, int e1, int s2, int e2) {
	return (s1 == s2 && e1 > e2);
}

int is_met(int s1, int e1, int s2, int e2) {
	return e1 == s2;
}

int is_met_by(int s1, int e1, int s2, int e2) {
	return s1 == e2;
}

int is_overlapped(int s1, int e1, int s2, int e2) {
	return (s1 < s2 && e1 > s2 && e1 < e2);
}

int is_overlapped_by(int s1, int e1, int s2, int e2) {
	return (s1 > s2 && s1 < e2 && e1 > e2);
}

int is_fully_contained_other(int s1, int e1, int s2, int e2) {
	return is_started(s1, e1, s2, e2) 
		|| is_during(s1, e1, s2, e2) 
		|| is_finished(s1, e1, s2, e2)
		|| is_equal(s1, e1, s2, e2)
		|| is_finished_by(s1, e1, s2, e2)
		|| is_contained(s1, e1, s2, e2)
		|| is_started_by(s1, e1, s2, e2);
}

int is_overlapped_other(int s1, int e1, int s2, int e2) {
	return is_fully_contained_other(s1, e1, s2, e2)
		|| is_overlapped(s1, e1, s2, e2)
		|| is_overlapped_by(s1, e1, s2, e2)
		|| is_met(s1, e1, s2, e2)
		|| is_met_by(s1, e1, s2, e2);
		
}

int run(state* s, char* input) {
	int s1, e1, s2, e2;
	parse(input, &s1, &e1, &s2, &e2);
	if (is_overlapped_other(s1, e1, s2, e2)) {
		s->count++;
		printf("%d %d %d %d\n", s1, e1, s2, e2);
	}
	return 0;
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
		if (run(s, line) != 0) {
			goto finalize;
		}
	}

	printf("%d\n", getcount(s));

finalize:
	fclose(f);

	return 1;
}




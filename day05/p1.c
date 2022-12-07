#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTACKNUM 32

char* debug;

typedef struct node node;
struct node {
	int id;
	node* next;
};

typedef struct stack {
	node *head;
	node *tail;
	int id;
} stack;

typedef struct state {
	stack stacks[MAXSTACKNUM];
	int stacks_loaded;
	int stacks_n;
} state;

char* getalltops(state *s) {
	static char tops[MAXSTACKNUM];
	memset(tops, 0, MAXSTACKNUM * sizeof(char));
	for (int i = 0; i < s->stacks_n; i++) {
		tops[i] = s->stacks[i].head->id;
	}
	tops[s->stacks_n] = 0;
	return tops;
}

void parse_node_id(char* s, char* id) {
	sscanf(s, "[%c]", id);
}

void report(state* s) {
	for (size_t i = 0; i < s->stacks_n; i++) {
		printf("stack %d: ", s->stacks[i].id);
		node* n = s->stacks[i].head;
		while (n != NULL) {
			printf("%c ", n->id);
			n = n->next;
		}
		printf("\n");
	}
}

void parse_stack_row(state* s, char* input) { 
	size_t len = strlen(input);
	for (size_t i = 0; i < len; i+=4) {
		char id = 0;
		parse_node_id(input + i, &id);
		if (id != 0) {
			node* node = malloc(sizeof(node));
			node->id = id;
			node->next = NULL;
			if (s->stacks[i/4].tail == NULL) {
				s->stacks[i/4].tail = node;
				s->stacks[i/4].head = node;
			} else {
				s->stacks[i/4].tail->next = node;
				s->stacks[i/4].tail = node;
			}
		}
	}
	s->stacks_n = len / 4;
}

void parse_stack_id(char* s, int* id) {
	sscanf(s, " %d ", id);
}

void parse_stack_ids(state* s, char* input) {
	size_t len = strlen(input);
	for (size_t i = 0; i < len; i+=4) {
		int id = 0;
		parse_stack_id(input + i, &id);
		s->stacks[i/4].id = id;
	}
}

void parse_stack_op(char* s, int* src_id, int* node_n, int* dest_id) {
	// given string like "move 1 from 8 to 1", parse node_n=1, src_id=8, dest_id=1
	sscanf(s, "move %d from %d to %d", node_n, src_id, dest_id);
}

void exec_stack_op(state* s, int src_id, int node_n, int dest_id) {
	// given node_n=1, src_id=8, dest_id=1, move node_n nodes from stack src_id to stack dest_id
	stack *src = NULL, *dest = NULL;
	for (size_t i = 0; i < s->stacks_n; i++) {
		if (s->stacks[i].id == src_id) {
			src = &(s->stacks[i]);
		} else if (s->stacks[i].id == dest_id) {
			dest = &(s->stacks[i]);
		}
	}
	node* tmp = NULL;
	for (size_t i = 0; i < node_n; i++) {
		tmp = src->head->next;
		src->head->next = dest->head;
		dest->head = src->head;
		src->head = tmp;
	}
}

int run(state* s, char* input) {
	if (strstr(input, "[") != NULL) {
		parse_stack_row(s, input);
	} else if (strstr(input, "m") != NULL) {
		if (debug) report(s);

		int a, b, c;
		parse_stack_op(input, &a, &b, &c);
		exec_stack_op(s, a, b, c);

		if (debug) printf("===>\n");
		if (debug) report(s);
		if (debug) printf("\n");
	} else if (input[0] == '\n') {
		// pass
	} else {
		parse_stack_ids(s, input);
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
	state *s = &((state) {{0}, 0, 0});

	// read the file line by line
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, f)) != -1) {
		if (run(s, line) != 0) {
			goto finalize;
		}
	}

	// TODO: some memory leak here, but I don't care. (shrug)

	printf("%s\n", getalltops(s));

finalize:
	fclose(f);

	return 1;
}



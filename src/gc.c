#include "../inc/ft_ping.h"

extern struct ping p;
struct ptrlist *gc = NULL;

void gc_free() {
	struct ptrlist *elem, *next;
	elem = gc;
	while (elem) {
		next = elem->next;
		free(elem->ptr);
		free(elem);
		elem = next;
	}
}

void gc_add(void *ptr) {
	struct ptrlist *new = malloc(sizeof(struct ptrlist));
	if (!new) {
		ping_error("malloc error\n");
	}
	new->ptr = ptr;
	new->next = NULL;
	if (!gc) {
		gc = new;
	}
	else {
		new->next = gc;
		gc = new;
	}
}

void ping_exit() {
	gc_free();
	if (p.socket) {
		close(p.socket);
	}
}

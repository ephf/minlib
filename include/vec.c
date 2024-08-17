#ifndef VEC__
#define VEC__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define vof(a, b...) ({\
		typeof(a) va[] = { a, b };\
		typeof(a)* v = 0;\
		for(int i = 0; i < sizeof(va) / sizeof(*va); i++)\
			push(&v, va[i]); \
		v; })

void push_(int** vec, void* item, int n) {
	if(!*vec) {
		if(!(*vec = (int*) calloc(sizeof(int[2]) + n, 1) + 2)) {
			perror("push()"); exit(1); }
		(*vec)[-2] = n; }
	if(((*vec)[-1] += n) > (*vec)[-2]) {
		while(((*vec)[-2] *= 2) < (*vec)[-2]);
		if(!(*vec = (int*) realloc(*vec - 2, sizeof(int[2]) + (*vec)[-2]))) {
			perror("push()"); exit(1); }
		*vec += 2; }
	memcpy((void*) *vec + (*vec)[-1] - n, item, n);
}

#define push(vec, item) \
	push_((void*)(vec), &(typeof(**(vec))) { item }, sizeof(**(vec)))

void pop_(int* vec, int n) {
	if((vec[-1] -= n) < 0) {
		fprintf(stderr, "pop(): removing %d byte(s) from a %d byte vector",
				n, vec[-1] + n);
		exit(1); }
}

#define pop(vec) \
	pop_((void*)(vec), sizeof(*(vec)))

#define vsz(vec) \
	(((int*)(void*)(vec))[-1] / sizeof(*(vec)))

void vfree(void* vec) { free(vec - sizeof(int[2])); }

#endif

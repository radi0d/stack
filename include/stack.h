#pragma once

#include <stddef.h>

typedef enum stack_error {
	OK,
	ALLOC_ERROR,
	EMPTY_ERROR,
	CANARY_ERROR,
} sterr_t;

const char *sterr_string(sterr_t);

typedef double stelem_t;

typedef struct stack {
	void *canary_buf;
	stelem_t *buf;
	size_t cap;
	size_t len;
} stack_t;

stack_t *stack_new(void);
void stack_free(stack_t *s);

sterr_t stack_push(stack_t *s, stelem_t e);
sterr_t stack_pop(stack_t *s, stelem_t *dst);

sterr_t stack_print(const stack_t *s);

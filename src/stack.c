#include "stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *
sterr_string(sterr_t e)
{
	switch(e) {
	case OK:
		return "OK";
	case ALLOC_ERROR:
		return "Allocation error";
	case EMPTY_ERROR:
		return "Stack is empty";
	default:
		assert("Unreachable" && 0);
	}

	return nullptr;
}

stack_t *
stack_new(void)
{
	stack_t *s = (stack_t *) calloc(1, sizeof(stack_t));
	if (!s)
		return nullptr;

	s->buf = (stelem_t *) calloc(1, sizeof(stelem_t));
	if (!s->buf) {
		free(s);
		return nullptr;
	}

	s->cap = 1;
	s->len = 0;

	return s;
}

void
stack_free(stack_t *s)
{
	assert(s);
	assert(s->buf);
	assert(s->len <= s->cap);

	free(s->buf);
	free(s);
}

sterr_t
stack_push(stack_t *s, stelem_t e)
{
	assert(s);
	assert(s->buf);
	assert(s->len <= s->cap);

	if (s->len == s->cap) {
		size_t cap = 1;
		while (cap < (s->len + 1))
			cap *= 2;

		stelem_t *tbuf = (stelem_t *) calloc(cap, sizeof(stelem_t));
		if (!tbuf)
			return ALLOC_ERROR;

		memcpy(tbuf, s->buf, s->len * sizeof(stelem_t));
		free(s->buf);
		s->buf = tbuf;
		s->cap = cap;
	}

	s->buf[s->len++] = e;

	return OK;
}

sterr_t
stack_pop(stack_t *s, stelem_t *dst)
{
	assert(s);
	assert(s->buf);
	assert(s->len <= s->cap);

	assert(dst);

	if (!s->len)
		return EMPTY_ERROR;

	*dst = s->buf[--(s->len)];

	return OK;
}

void
stack_print(const stack_t *s)
{
	assert(s);
	assert(s->buf);
	assert(s->len <= s->cap);

	if (!s->len) {
		printf("%s\n", sterr_string(EMPTY_ERROR));
		return;
	}

	for (size_t i = 0; i < s->len; i++)
		printf("%lg ", s->buf[i]);
	printf("<\n");
}

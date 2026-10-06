#include "stack.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANARY_LEFT 0xdeadbeef
#define CANARY_RIGHT 0xdefec8ed

#define CANARY_CHECK(s) \
	do {                                                                   \
		const uint8_t *t = (const uint8_t *) s->canary_buf;            \
		if (CANARY_LEFT != *((const uint32_t *) t))                    \
			return CANARY_ERROR;                                   \
		if (CANARY_RIGHT != *((const uint32_t *)                       \
		                      (t + sizeof(int32_t)                     \
		                         + sizeof(stelem_t) * s->cap)))        \
			return CANARY_ERROR;                                   \
	} while(false)

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
	case CANARY_ERROR:
		return "Canary corruption";
	default:
		assert("Unreachable" && 0);
	}

	return nullptr;
}

static void *
canary_alloc(size_t n, size_t size)
{
	uint8_t *buf = calloc(sizeof(uint32_t) * 2 + n * size, 1);
	if (!buf)
		return nullptr;

	*((uint32_t *) buf) = CANARY_LEFT;
	*((uint32_t *) (buf + sizeof(uint32_t) + n * size)) = CANARY_RIGHT;

	return buf;
}

stack_t *
stack_new(void)
{
	stack_t *s = (stack_t *) calloc(1, sizeof(stack_t));
	if (!s)
		return nullptr;

	s->canary_buf = canary_alloc(1, sizeof(stelem_t));
	if (!s->canary_buf) {
		free(s);
		return nullptr;
	}

	s->buf = (stelem_t *) ((uint32_t *) s->canary_buf + 1);

	s->cap = 1;
	s->len = 0;

	return s;
}

void
stack_free(stack_t *s)
{
	assert(s);
	assert(s->canary_buf);
	assert(s->buf);
	assert(s->len <= s->cap);

	free(s->canary_buf);
	free(s);
}

sterr_t
stack_push(stack_t *s, stelem_t e)
{
	assert(s);
	assert(s->canary_buf);
	assert(s->buf);
	assert(s->len <= s->cap);

	CANARY_CHECK(s);

	if (s->len == s->cap) {
		size_t cap = 1;
		while (cap < (s->len + 1))
			cap *= 2;

		uint32_t *cbuf = (uint32_t *) canary_alloc(cap, sizeof(stelem_t));
		if (!cbuf)
			return ALLOC_ERROR;

		stelem_t *tbuf = (stelem_t *) (cbuf + 1);

		memcpy(tbuf, s->buf, s->len * sizeof(stelem_t));
		free(s->canary_buf);
		s->canary_buf = cbuf;
		s->buf = tbuf;
		s->cap = cap;
	}

	s->buf[s->len++] = e;

	CANARY_CHECK(s);

	return OK;
}

sterr_t
stack_pop(stack_t *s, stelem_t *dst)
{
	assert(s);
	assert(s->canary_buf);
	assert(s->buf);
	assert(s->len <= s->cap);

	assert(dst);

	CANARY_CHECK(s);

	if (!s->len)
		return EMPTY_ERROR;

	*dst = s->buf[--(s->len)];

	CANARY_CHECK(s);

	return OK;
}

sterr_t
stack_print(const stack_t *s)
{
	assert(s);
	assert(s->canary_buf);
	assert(s->buf);
	assert(s->len <= s->cap);

	CANARY_CHECK(s);

	if (!s->len) {
		printf("%s\n", sterr_string(EMPTY_ERROR));
		return OK;
	}

	for (size_t i = 0; i < s->len; i++)
		printf("%lg ", s->buf[i]);
	printf("<\n");

	CANARY_CHECK(s);

	return OK;
}

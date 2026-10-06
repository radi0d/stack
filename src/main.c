#include "stack.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int
main(void)
{
	stack_t *s = stack_new();
	if (!s) {
		fprintf(stderr, "[ERR] %s\n", sterr_string(ALLOC_ERROR));
		return 1;
	}

	while (true) {
		printf("1 - push, 2 - pop, 3 - print, 4 - exit: ");
		size_t choice = 0;
		if (EOF == scanf("%lu", &choice))
			break;

		switch(choice) {
		case 1: { // push
			stelem_t d = NAN;

			while (true) {
				printf("Enter the value: ");
				scanf("%lg", &d);
				if (!isnan(d))
					break;
			}

			sterr_t err = stack_push(s, d);

			switch (err) {
			case ALLOC_ERROR:
				printf("[ERR] %s\n", sterr_string(err));
				return 1;
			case OK:
				break;
			case EMPTY_ERROR:
			case CANARY_ERROR:
				printf("[ERR] %s\n", sterr_string(err));
				return 1;
			default:
				assert("Unreachable" && 0);
			}

			break;
		}
		case 2: { // pop
			stelem_t res = 0;
			sterr_t err = stack_pop(s, &res);

			switch (err) {
			case EMPTY_ERROR:
				printf("%s\n", sterr_string(err));
				break;
			case OK:
				printf("Popped value = %lg\n", res);
				break;
			case CANARY_ERROR:
				printf("[ERR] %s\n", sterr_string(err));
				return 1;
			case ALLOC_ERROR:
			default:
				assert("Unreachable" && 0);
			}

			break;
		}
		case 3: // print
			sterr_t err = stack_print(s);
			switch (err) {
			case OK:
				break;
			case CANARY_ERROR:
				printf("[ERR] %s\n", sterr_string(err));
				return 1;
			case ALLOC_ERROR:
			case EMPTY_ERROR:
			default:
				assert("Unreachable" && 0);
			}

			break;
		case 4: // exit
			goto exit;
		default:
			break;
		}
	}

exit:
	stack_free(s);

	return 0;
}
